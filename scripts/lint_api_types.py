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
    """
    Recursively reads a header file and all of its base classes.
    """
    if visited is None:
        visited = set()
        
    if filepath.name in visited:
        return ""
    visited.add(filepath.name)
    
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception as e:
        print(f"Could not read {filepath}: {e}")
        return ""
        
    clean_content = strip_cpp_comments(content)
    
    # 1. Grab everything between the class declaration and the opening brace
    # Example: class PassportElementErrorFiles : public PassportElementError {
    class_decl_pattern = re.compile(r'class\s+[A-Za-z0-9_]+\s*:([^\{]+)\{', re.DOTALL)
    decl_match = class_decl_pattern.search(clean_content)
    
    if decl_match:
        inheritance_list = decl_match.group(1)
        
        # 2. Extract words immediately following access specifiers
        base_classes = re.findall(r'(?:public|protected|private|virtual)\s+([A-Za-z0-9_:]+)', inheritance_list)
        
        for base_class in base_classes:
            # Strip out namespace if it exists (e.g., tgbot::PassportElementError -> PassportElementError)
            base_class_name = base_class.split(':')[-1]
            
            # Ignore standard C++ templates/types
            if base_class_name.startswith("std") or base_class_name == "enable_shared_from_this":
                continue
                
            base_filename = f"{base_class_name}.h"
            matched_bases = list(target_dir.rglob(base_filename))
            
            if matched_bases:
                base_content = get_full_class_content(matched_bases[0], target_dir, visited)
                clean_content += "\n" + base_content
            
    return clean_content

def lint_telegram_json_fields(json_filepath, directory_path):
    json_path = Path(json_filepath)
    target_dir = Path(directory_path)

    if not json_path.is_file():
        print(f"Error: JSON file '{json_filepath}' not found.")
        return

    if not target_dir.is_dir():
        print(f"Error: '{directory_path}' is not a valid directory.")
        return

    with open(json_path, 'r', encoding='utf-8') as f:
        try:
            api_data = json.load(f)
        except json.JSONDecodeError as e:
            print(f"Error parsing JSON: {e}")
            return

    types_data = api_data.get("types", {})
    issues_found = 0

    for type_name, type_info in types_data.items():
        header_filename = f"{type_name}.h"
        matched_files = list(target_dir.rglob(header_filename))

        if not matched_files:
            print(f"{target_dir}:(0) Missing header file entirely for type `{type_name}` ({header_filename})")
            issues_found += 1
            continue

        filepath = matched_files[0]

        # Fetch content (now properly following inheritance)
        clean_content = get_full_class_content(filepath, target_dir)
        fields = type_info.get("fields", [])

        for field in fields:
            original_field_name = field.get("name")
            camel_field_name = snake_to_camel(original_field_name)
            
            pattern = re.compile(rf'\b{re.escape(camel_field_name)}\b\s*[;,]')
            
            if not pattern.search(clean_content):
                print(f"{filepath}:(1) `{camel_field_name}` (from `{original_field_name}`) field is missing from class `{type_name}` or its base classes")
                issues_found += 1

    if issues_found == 0:
        print("✅ All C++ headers match the JSON specification perfectly!")
    else:
        print(f"\n❌ Finished with {issues_found} missing file(s)/field(s).")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Lint C++ headers against a Telegram API JSON spec (supports inheritance).")
    parser.add_argument("json_file", type=str, help="Path to the Telegram API JSON file")
    parser.add_argument("directory", type=str, help="Path to the directory containing .h files")
    
    args = parser.parse_args()
    lint_telegram_json_fields(args.json_file, args.directory)