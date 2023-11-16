import json

def generate_cmake_lists(compilation_database_path):
    with open(compilation_database_path, 'r') as f:
        compilation_database = json.load(f)

    cmake_lists_content = "cmake_minimum_required(VERSION 3.5)\n\n"
    cmake_lists_content += "project(YourProjectName)\n\n"

    source_files = set()

    for entry in compilation_database:
        source_file = entry['file']
        source_files.add(source_file)

    cmake_lists_content += "add_executable(YourExecutableName\n"
    for source_file in source_files:
        cmake_lists_content += "    " + source_file + "\n"
    cmake_lists_content += ")\n"

    return cmake_lists_content

def save_cmake_lists(cmake_lists_content, output_file_path):
    with open(output_file_path, 'w') as f:
        f.write(cmake_lists_content)

if __name__ == "__main__":
    compilation_database_path = "compile_commands.json"
    output_file_path = "CMakeLists.txt"

    cmake_lists_content = generate_cmake_lists(compilation_database_path)
    save_cmake_lists(cmake_lists_content, output_file_path)

    print(f"CMakeLists.txt generated successfully at {output_file_path}")
