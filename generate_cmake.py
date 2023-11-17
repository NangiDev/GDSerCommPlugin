'''
Generate a CMakeLists from the compilation database if it exists.
'''

import json


def generate_cmake_lists(db_path):
    '''
    Will return a CMakeLists file containing the data from compilation database

    Parameters:
    db_path (string): Path to the compilation_database.json file

    Returns:
    string: Content of the compilation db in CMakeLists style
    '''
    with open(db_path, 'r', encoding="utf-8") as f:
        compilation_database = json.load(f)
        f.close()

    project_name = "sercomm"
    content = "cmake_minimum_required(VERSION 3.5)\n\n"
    content += f"project({project_name})\n\n"

    source_files = set()

    for entry in compilation_database:
        source_file = entry['file']
        source_files.add(source_file)

    content += f"add_executable({project_name}\n"
    for source_file in source_files:
        content += "    " + source_file + "\n"
    content += ")\n"

    return content


def save_cmake_lists(content, out_path):
    '''
    Will write the given content to given output path.

    Parameters:
    content (string): Content to be written
    out_path (string): Path to where to write the file
    '''
    with open(out_path, 'w', encoding="utf-8") as f:
        f.write(content)
        f.close()


if __name__ == "__main__":
    COMPILATION_DATABASE_PATH = "compile_commands.json"
    OUTPUT_FILE_PATH = "CMakeLists.txt"

    cmake_lists_content = generate_cmake_lists(COMPILATION_DATABASE_PATH)
    save_cmake_lists(cmake_lists_content, OUTPUT_FILE_PATH)

    print(f"CMakeLists.txt generated successfully at {OUTPUT_FILE_PATH}")
