# MBED Utitilty / Driver Extension Library

## Adding as a git submmodule to your project

If you want the repo to live in the base directory of the parent repo, just run

```git submodule add https://github.com/scottc11/ok-drivers.git```

Otherwise, specifiy a directory

```git submodule add https://github.com/scottc11/ok-drivers.git <path-to-directory>/ok-drivers```

### [Git Submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules)

# Making Changes

First, `cd` into the submodule directory and checkout a new branch with `git checkout -b myBranchName`

You can now commit changes and push to the `remote`

### documenting / add comments
Use the [doxygen syntax](https://www.doxygen.nl/manual/commands.html) for adding method descriptions etc. 