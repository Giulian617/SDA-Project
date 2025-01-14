import subprocess

def generate_test(filename, generator_path, solution_path, args):
    input = filename + ".in"
    output = filename + ".ok"

    command = "./" + generator_path + " " + args + " > " + input
    result = subprocess.run(command, shell=True)
    if result.returncode != 0:
        print("Error generating test " + filename + ".")
        print("Command: " + " ".join(command))
        print(result.stderr)
        exit(1)

    print("Generated test " + filename)

    command = "./" + solution_path + " < " + input + " > " + output
    result = subprocess.run(command, shell=True)
    if result.returncode != 0:
        print("Error generating test " + filename + ".")
        print("Command: " + " ".join(command))
        print(result.stderr)
        exit(1)

    print("Run solution on " + filename)
    return filename

def compile(filenames, args, out):
    assert len(filenames) == len(out)
    for i in range(len(filenames)):
        print("Compiling " + filenames[i] + "... to " + out[i])
        command = "g++ " + filenames[i] + " " + args + " " + out[i]
        result = subprocess.run(command, shell=True)
        if result.returncode != 0:
            print("Error compiling " + filenames[i] + ".")
            print("Command: " + " ".join(command))
            print(result.stderr)
            exit(1)

def generate():
    compile(
        ["final.cpp", "gen_random_insert_random_contains.cpp", "gen_range_sum.cpp", "all_in_one_gen.cpp"], 
        "-O2 -std=c++20 -o", 
        ["solution", "gen_insert_contains", "gen_in_range", "gen_all"]
    )

    generators = ["gen_insert_contains", "gen_in_range", "gen_all"]
    prefix = ["random_insert_random_contains", "random_insert_in_range", "all_ops"]
    
    assert len(generators) == len(prefix)

    args = [
        [
            [200_000, 1_000_000, 0, 34587], 
            [500_000, 1_000_000, 0, 34578], 
            [1_000_000, 1_000_000, 1, 3339], 
            [1_000_000, 1_000_000, 0, 34587]
        ], 
        [
            [500_000, 1_000_000, 10, 378], 
            [200_000, 1_000_000, 50, 57384], 
            [500_000, 1_000_000, 100, 6777], 
            [100_000, 1_000_000, 100, 38945], 
            [900_000, 1_000_000, 1000, 38945]
        ], 
        [
            [1_000_000, 1_000_000, 0, 34587],
            [1_000_000, 1_000_000, 0, 5436999],
            [500_000, 1_000_000, 1, 22],
            [200_000, 1_000_000, 1, 1109],
        ]
    ]

    filesnames = []
    for i in range(len(generators)):
        generator = generators[i]
        for j in range(len(args[i])):
            filename = "tests/" + prefix[i] + "_" + str(j)
            generate_test(filename, generator, "solution", " ".join(map(str, args[i][j])))

    command = "zip tests.zip tests/*.in"
    result = subprocess.run(command, shell=True)
    if result.returncode != 0:
        print("Error zipping tests.")
        print("Command: " + " ".join(command))
        print(result.stderr)
        exit(1)    

    print("Generated all tests. Zipped them to tests.zip")

generate()