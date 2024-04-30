# LogiCalc
A boolean expression calculator written in C++ as a project for EECS 348.

Run the  `./example/example.py` file to get an idea of how the program should work. You can also look at the code to see the general structure of the program.

# Developer

## Making Changes

Refer to the [workflow doc](doc/workflow.md) for information on how to add changes. It is essentially the [Gitflow Workflow](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow) if you are familar. Just make sure to create your own branch, make changes in that branch, and create a [pull request](https://github.com/aaatipamula/bool_expr_calc/pulls) to have your changes merged.

## Setting Up

All you really need is **g++** and **make** installed. This is pretty standard for any Linux machine, including the lab machines. If you are running this on Windows its going to take some extra configuration and setup to work.

First navigate to the root of the project (the folder name on your computer). After that just run the following to build the project:

```bash
make
```

The program accepts one command line argument as such:

```
./bin/lcalc "T | F"
```

