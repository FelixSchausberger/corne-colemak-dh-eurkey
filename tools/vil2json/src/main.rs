use anyhow::{Context, Result};
use clap::Parser;
use serde::{Deserialize, Serialize};
use serde_json::Value;
use std::path::PathBuf;

/// Convert VIAL .vil keyboard layout files to QMK keymap.json format
#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    /// Input VIAL .vil file path
    input: PathBuf,

    /// Output keymap.json file path (defaults to input with .json extension)
    #[arg(short, long)]
    output: Option<PathBuf>,

    /// QMK keyboard identifier
    #[arg(short = 'k', long, default_value = "crkbd/rev4_1/standard")]
    keyboard: String,

    /// QMK layout macro name
    #[arg(short, long, default_value = "LAYOUT_split_3x6_3_ex2")]
    layout: String,

    /// Maximum number of layers to include (filters empty layers at the end)
    #[arg(short = 'm', long)]
    max_layers: Option<usize>,

    /// Skip completely empty layers (all KC_TRNS or KC_NO)
    #[arg(short = 'f', long)]
    filter_empty: bool,
}

#[derive(Debug, Deserialize)]
struct VialData {
    layout: Vec<Vec<Vec<Value>>>, // 3D: layers[rows[keys]]
    #[serde(default)]
    #[allow(dead_code)]
    version: u32,
    #[serde(default)]
    #[allow(dead_code)]
    uid: u64,
}

#[derive(Debug, Serialize)]
struct QmkKeymap {
    version: u32,
    keyboard: String,
    layout: String,
    layers: Vec<Vec<String>>,
}

fn main() -> Result<()> {
    let args = Args::parse();

    // Read VIAL file
    let vil_content = std::fs::read_to_string(&args.input)
        .with_context(|| format!("Failed to read VIAL file: {}", args.input.display()))?;

    let vil_data: VialData =
        serde_json::from_str(&vil_content).context("Failed to parse VIAL JSON")?;

    // Flatten layers and filter
    let mut layers: Vec<Vec<String>> = vil_data.layout.into_iter().map(flatten_layer).collect();

    // Apply max_layers limit
    if let Some(max) = args.max_layers {
        layers.truncate(max);
    }

    // Filter empty layers if requested
    if args.filter_empty {
        layers.retain(|layer| !is_layer_empty(layer));
    }

    let layer_count = layers.len();
    let key_count = if !layers.is_empty() {
        layers[0].len()
    } else {
        0
    };

    // Build QMK keymap structure
    let qmk_keymap = QmkKeymap {
        version: 1,
        keyboard: args.keyboard,
        layout: args.layout,
        layers,
    };

    // Determine output path
    let output_path = args.output.unwrap_or_else(|| {
        let mut path = args.input.clone();
        path.set_extension("json");
        path
    });

    // Write output
    let output_json =
        serde_json::to_string_pretty(&qmk_keymap).context("Failed to serialize QMK keymap")?;

    std::fs::write(&output_path, output_json)
        .with_context(|| format!("Failed to write output file: {}", output_path.display()))?;

    println!(
        "✓ Converted {} layers with {} keys each",
        layer_count, key_count
    );
    println!("✓ Saved to {}", output_path.display());

    Ok(())
}

/// Flatten a 2D layer matrix into a 1D array, removing -1 placeholders
/// VIAL stores split keyboards as 8 rows: left side (0-3), right side (4-7)
/// QMK expects keys ordered by physical rows: row0_left + row0_right, row1_left + row1_right, etc.
fn flatten_layer(layer_matrix: Vec<Vec<Value>>) -> Vec<String> {
    // Split into left (rows 0-3) and right (rows 4-7) sides
    let (left_rows, right_rows) = layer_matrix.split_at(4);

    let mut result = Vec::new();

    // For each row index, take left row then corresponding right row
    for i in 0..4 {
        // Add left row keys (left-to-right, filter -1 placeholders)
        for key in &left_rows[i] {
            match key {
                Value::Number(n) if n.as_i64() == Some(-1) => {}
                Value::String(s) => result.push(s.clone()),
                _ => {}
            }
        }

        // Add right row keys in REVERSE (split keyboard is mirrored, filter -1 placeholders)
        for key in right_rows[i].iter().rev() {
            match key {
                Value::Number(n) if n.as_i64() == Some(-1) => {}
                Value::String(s) => result.push(s.clone()),
                _ => {}
            }
        }
    }

    result
}

/// Check if a layer contains only transparent or empty keys
fn is_layer_empty(layer: &[String]) -> bool {
    layer
        .iter()
        .all(|key| key == "KC_TRNS" || key == "KC_NO" || key.is_empty())
}

#[cfg(test)]
mod tests {
    use super::*;
    use serde_json::json;

    #[test]
    fn test_flatten_layer() {
        // Test with proper 8-row split keyboard structure (4 left + 4 right)
        // Left side: row0=[Q,W], row1=[A,S], row2=[Z], row3=[]
        // Right side: row0=[Y,U], row1=[O,I], row2=[slash], row3=[]
        // Expected: row0_left + row0_right_reversed, row1_left + row1_right_reversed, ...
        // Result: [Q, W, U, Y, A, S, I, O, Z, slash]
        let layer = vec![
            // Left rows (0-3)
            vec![json!("KC_Q"), json!("KC_W"), json!(-1)],
            vec![json!("KC_A"), json!(-1), json!("KC_S")],
            vec![json!("KC_Z")],
            vec![],
            // Right rows (4-7) - stored inner-to-outer, will be reversed
            vec![json!("KC_Y"), json!("KC_U"), json!(-1)],
            vec![json!("KC_O"), json!(-1), json!("KC_I")],
            vec![json!("KC_SLASH")],
            vec![],
        ];

        let flattened = flatten_layer(layer);
        assert_eq!(
            flattened,
            vec![
                "KC_Q", "KC_W", "KC_U", "KC_Y", "KC_A", "KC_S", "KC_I", "KC_O", "KC_Z", "KC_SLASH"
            ]
        );
    }

    #[test]
    fn test_is_layer_empty() {
        assert!(is_layer_empty(&vec![
            "KC_TRNS".to_string(),
            "KC_NO".to_string()
        ]));
        assert!(!is_layer_empty(&vec![
            "KC_Q".to_string(),
            "KC_TRNS".to_string()
        ]));
    }
}
