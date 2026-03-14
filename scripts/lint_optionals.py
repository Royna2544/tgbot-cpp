import re
import argparse
from pathlib import Path

def lint_telegram_headers(directory_path, auto_fix=False):
    # THE PATCHED REGEX:
    # Group 1 & 2: Full comment block + trailing whitespace, and inner comment text
    # Group 3: The C++ type (Matches anything that is NOT a semicolon, non-greedy)
    # Group 4: The whitespace before the variable name
    # Group 5: The variable name itself
    # Group 6: Any optional assignment (e.g., = false) plus the semicolon
    pattern = re.compile(r'(/\*\*(.*?)\*/\s*)([^;]+?)(\s+)([a-zA-Z0-9_]+)((?:\s*=[^;]+)?\s*;)', re.DOTALL)
    
    target_dir = Path(directory_path)
    
    if not target_dir.is_dir():
        print(f"Error: '{directory_path}' is not a valid directory.")
        return

    for filepath in target_dir.rglob('*.h'):
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
        except Exception as e:
            print(f"Could not read {filepath}: {e}")
            continue
            
        matches = list(pattern.finditer(content))
        fixes_applied = 0
        
        for match in reversed(matches):
            comment_block = match.group(1)
            comment_text = match.group(2).lower()
            
            # .strip() handles any stray spaces caught by the broad non-semicolon match
            cpp_type = match.group(3).strip() 
            whitespace = match.group(4)
            var_name = match.group(5).strip()
            semicolon_and_assignment = match.group(6)
            
            line_number = content.count('\n', 0, match.start(5)) + 1
            
            # The core checks
            is_optional_in_comment = "optional." in comment_text
            is_wrapped_in_optional = cpp_type.startswith("std::optional")
            
            if is_optional_in_comment and not is_wrapped_in_optional:
                print(f"{filepath}:({line_number}) `{var_name}` is missing std::optional (Current type: {cpp_type})")
                
                if auto_fix:
                    new_type = f"std::optional<{cpp_type}>"
                    new_string = f"{comment_block}{new_type}{whitespace}{var_name}{semicolon_and_assignment}"
                    content = content[:match.start()] + new_string + content[match.end():]
                    fixes_applied += 1
                    
            elif not is_optional_in_comment and is_wrapped_in_optional:
                print(f"{filepath}:({line_number}) `{var_name}` is wrongly using std::optional")
                
                if auto_fix:
                    inner_type_match = re.match(r'std::optional<\s*(.*?)\s*>', cpp_type)
                    if inner_type_match:
                        new_type = inner_type_match.group(1)
                        new_string = f"{comment_block}{new_type}{whitespace}{var_name}{semicolon_and_assignment}"
                        content = content[:match.start()] + new_string + content[match.end():]
                        fixes_applied += 1

        # Check for missing <optional> include
        uses_optional = "std::optional" in content
        has_include = re.search(r'#include\s*<optional>', content)

        if uses_optional and not has_include:
            print(f"{filepath}:(1) Missing `#include <optional>`")
            
            if auto_fix:
                include_matches = list(re.finditer(r'^#include\s+.*$', content, re.MULTILINE))
                if include_matches:
                    last_include = include_matches[-1]
                    insert_pos = last_include.end()
                    content = content[:insert_pos] + "\n#include <optional>" + content[insert_pos:]
                else:
                    define_match = re.search(r'^#define\s+\w+$', content, re.MULTILINE)
                    if define_match:
                        insert_pos = define_match.end()
                        content = content[:insert_pos] + "\n\n#include <optional>" + content[insert_pos:]
                    else:
                        content = "#include <optional>\n" + content
                
                fixes_applied += 1

        if auto_fix and fixes_applied > 0:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"🔧 Automatically fixed {fixes_applied} issue(s) in {filepath.name}\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Lint and auto-fix Telegram Bot C++ headers for std::optional correctness.")
    parser.add_argument("directory", type=str, help="Path to the directory containing .h files")
    parser.add_argument("--fix", action="store_true", help="Automatically fix missing/extra std::optional wrappers and missing includes")
    
    args = parser.parse_args()
    lint_telegram_headers(args.directory, auto_fix=args.fix)