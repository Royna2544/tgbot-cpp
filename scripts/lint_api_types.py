import json
import argparse
import re
from pathlib import Path

def strip_cpp_comments(code):
    """Removes // and /* */ comments to prevent false positives."""
    code = re.sub(r'/\*.*?\*/', '', code, flags=re.DOTALL)
    code = re.sub(r'//.*', '', code)
    return code

def snake_to_camel(snake_str):
    """Converts snake_case to camelCase (e.g., a_b_c -> aBC)."""
    components = snake_str.split('_')
    return components[0] + ''.join(x.title() for x in components[1:])

def get_full_class_content(filepath, target_dir, visited=None):
    """Recursively reads a header file and all of its base classes."""
    if visited is None:
        visited = set()
        
    if filepath.name in visited:
        return ""
    visited.add(filepath.name)
    
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception:
        return ""
        
    clean_content = strip_cpp_comments(content)
    
    class_decl_pattern = re.compile(r'class\s+[A-Za-z0-9_]+\s*:([^\{]+)\{', re.DOTALL)
    decl_match = class_decl_pattern.search(clean_content)
    
    if decl_match:
        inheritance_list = decl_match.group(1)
        base_classes = re.findall(r'(?:public|protected|private|virtual)\s+([A-Za-z0-9_:]+)', inheritance_list)
        
        for base_class in base_classes:
            base_class_name = base_class.split(':')[-1]
            if base_class_name.startswith("std") or base_class_name == "enable_shared_from_this":
                continue
                
            base_filename = f"{base_class_name}.h"
            matched_bases = list(target_dir.rglob(base_filename))
            
            if matched_bases:
                base_content = get_full_class_content(matched_bases[0], target_dir, visited)
                clean_content += "\n" + base_content
            
    return clean_content

def generate_html_report(report_data, output_filepath):
    """Generates a styled HTML dashboard from the linting data."""
    html = """<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Telegram Bot API - Lint Report</title>
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; line-height: 1.6; color: #333; max-width: 1200px; margin: 0 auto; padding: 20px; background-color: #f8f9fa; }
        h1 { border-bottom: 2px solid #0088cc; padding-bottom: 10px; color: #0088cc; }
        h2 { margin-top: 30px; color: #444; }
        .card { background: #fff; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); padding: 20px; margin-bottom: 20px; }
        table { width: 100%; border-collapse: collapse; margin-top: 10px; }
        th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }
        th { background-color: #f1f3f5; font-weight: 600; }
        .badge { display: inline-block; padding: 4px 8px; border-radius: 4px; font-size: 12px; font-weight: bold; }
        .badge.required { background-color: #ffe3e3; color: #c92a2a; }
        .badge.optional { background-color: #e3fafc; color: #0b7285; }
        .badge.type { background-color: #f3f0ff; color: #5f3dc4; margin-right: 4px;}
        .file-path { font-family: monospace; color: #e64980; background: #fff0f6; padding: 2px 6px; border-radius: 4px;}
        .success { color: #2b8a3e; background: #e8fdf5; padding: 20px; border-radius: 8px; border: 1px solid #b2f2bb; }
    </style>
</head>
<body>
    <h1>Telegram Bot API - C++ Wrapper Lint Report</h1>
"""
    
    if not report_data["missing_files"] and not report_data["missing_fields"]:
        html += '<div class="success"><h2>✅ Perfect Score!</h2><p>All C++ headers match the JSON specification perfectly. No missing files or fields.</p></div>'
    
    # Render Missing Files Section (Now with full tables!)
    if report_data["missing_files"]:
        html += f"""<h2>❌ Missing Header Files ({len(report_data['missing_files'])})</h2>"""
        for missing in report_data["missing_files"]:
            html += f"""
            <div class="card">
                <h3>{missing['type_name']}</h3>
                <p style="font-size: 0.9em; color: #666;">{missing['description']}</p>
                <table>
                    <thead>
                        <tr>
                            <th>C++ Name</th>
                            <th>JSON Name</th>
                            <th>Status</th>
                            <th>Type(s)</th>
                            <th>API Description</th>
                        </tr>
                    </thead>
                    <tbody>
            """
            for field in missing["fields"]:
                req_class = "required" if field['required'] else "optional"
                req_text = "Required" if field['required'] else "Optional"
                types_html = "".join([f'<span class="badge type">{t}</span>' for t in field['types']])
                
                html += f"""
                        <tr>
                            <td><strong>{field['camel_name']}</strong></td>
                            <td><code>{field['original_name']}</code></td>
                            <td><span class="badge {req_class}">{req_text}</span></td>
                            <td>{types_html}</td>
                            <td style="font-size: 0.9em;">{field['description']}</td>
                        </tr>
                """
            html += """
                    </tbody>
                </table>
            </div>
            """

    # Render Missing Fields Section
    if report_data["missing_fields"]:
        html += f"""<h2>⚠️ Missing Fields in Existing Files ({sum(len(v['fields']) for v in report_data['missing_fields'].values())})</h2>"""
        for type_name, data in report_data["missing_fields"].items():
            html += f"""
            <div class="card">
                <h3>{type_name}</h3>
                <p>File: <span class="file-path">{data['filepath']}</span></p>
                <p style="font-size: 0.9em; color: #666;">{data['description']}</p>
                <table>
                    <thead>
                        <tr>
                            <th>C++ Name</th>
                            <th>JSON Name</th>
                            <th>Status</th>
                            <th>Type(s)</th>
                            <th>API Description</th>
                        </tr>
                    </thead>
                    <tbody>
            """
            for field in data["fields"]:
                req_class = "required" if field['required'] else "optional"
                req_text = "Required" if field['required'] else "Optional"
                types_html = "".join([f'<span class="badge type">{t}</span>' for t in field['types']])
                
                html += f"""
                        <tr>
                            <td><strong>{field['camel_name']}</strong></td>
                            <td><code>{field['original_name']}</code></td>
                            <td><span class="badge {req_class}">{req_text}</span></td>
                            <td>{types_html}</td>
                            <td style="font-size: 0.9em;">{field['description']}</td>
                        </tr>
                """
            html += """
                    </tbody>
                </table>
            </div>
            """

    html += """
</body>
</html>
"""
    with open(output_filepath, 'w', encoding='utf-8') as f:
        f.write(html)
    print(f"\n📄 HTML Report generated successfully: {output_filepath}")

def lint_telegram_json_fields(json_filepath, directory_path, report_filepath=None):
    json_path = Path(json_filepath)
    target_dir = Path(directory_path)

    if not json_path.is_file() or not target_dir.is_dir():
        print("Error: Invalid JSON file or directory path.")
        return

    with open(json_path, 'r', encoding='utf-8') as f:
        api_data = json.load(f)

    types_data = api_data.get("types", {})
    issues_found = 0
    
    report_data = {
        "missing_files": [],
        "missing_fields": {}
    }

    for type_name, type_info in types_data.items():
        header_filename = f"{type_name}.h"
        matched_files = list(target_dir.rglob(header_filename))
        
        type_description = " ".join(type_info.get("description", []))

        # 1. Handle completely missing headers
        if not matched_files:
            print(f"{target_dir}:(0) Missing header file entirely for type `{type_name}` ({header_filename})")
            issues_found += 1
            
            # Extract FULL field data for the report instead of just names
            missing_file_fields = []
            for field in type_info.get("fields", []):
                missing_file_fields.append({
                    "original_name": field.get("name"),
                    "camel_name": snake_to_camel(field.get("name")),
                    "types": field.get("types", []),
                    "required": field.get("required", False),
                    "description": field.get("description", "")
                })

            report_data["missing_files"].append({
                "type_name": type_name,
                "description": type_description,
                "fields": missing_file_fields
            })
            continue

        filepath = matched_files[0]
        clean_content = get_full_class_content(filepath, target_dir)
        fields = type_info.get("fields", [])

        # 2. Handle missing fields in existing headers
        for field in fields:
            original_field_name = field.get("name")
            camel_field_name = snake_to_camel(original_field_name)
            
            pattern = re.compile(rf'\b{re.escape(camel_field_name)}\b\s*[;,]')
            
            if not pattern.search(clean_content):
                print(f"{filepath}:(1) `{camel_field_name}` (from `{original_field_name}`) field is missing from class `{type_name}`")
                issues_found += 1
                
                if type_name not in report_data["missing_fields"]:
                    report_data["missing_fields"][type_name] = {
                        "filepath": str(filepath.name),
                        "description": type_description,
                        "fields": []
                    }
                    
                report_data["missing_fields"][type_name]["fields"].append({
                    "original_name": original_field_name,
                    "camel_name": camel_field_name,
                    "types": field.get("types", []),
                    "required": field.get("required", False),
                    "description": field.get("description", "")
                })

    if issues_found == 0:
        print("\n✅ All C++ headers match the JSON specification perfectly!")
    else:
        print(f"\n❌ Finished with {issues_found} missing file(s)/field(s).")
        
    if report_filepath:
        generate_html_report(report_data, report_filepath)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Lint C++ headers against a Telegram API JSON spec and optionally generate an HTML report.")
    parser.add_argument("json_file", type=str, help="Path to the Telegram API JSON file")
    parser.add_argument("directory", type=str, help="Path to the directory containing .h files")
    parser.add_argument("--generate-report", type=str, default=None, help="Filepath to save the structured HTML summary report (e.g., report.html)")
    
    args = parser.parse_args()
    lint_telegram_json_fields(args.json_file, args.directory, args.generate_report)