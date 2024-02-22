# Workflow

This document describes how to add or make changes to this project. We will be using a user-branching git workflow to manage changes to the project.

## Setup

Install [git](https://git-scm.com/downloads) if you haven't yet. If you're on windows I suggest installing [WSL2](https://learn.microsoft.com/en-us/windows/wsl/install) this is a Linux VM which makes developing in/with Linux much easier.

### SSH-Access

1. Generate an SSH key with the following command:

```sh
ssh-keygen -t rsa -b 4090
```

Follow along with the prompts you are given, or just hit enter a few times to default everything.

2. Copy your *public-key*

This should be located in `~/.ssh/id_rsa.pub` if you are on Linux. If you are on Windows this should be in `"C:\Users\<Your-Username>\.ssh\id_rsa.pub"`

If you changed the name of your file replace `id_rsa` with the name of your file. Windows may treat this file as a Micosoft Publisher file, but its just a regular text file. Open it with notepad or another text editor.

> WARNING: Do NOT copy the regular `id_rsa` file. This is a *private key* which should never be shared, copied or given out to **anyone**. 

3. Add your public-key to GitHub

Go to https://github.com and open your settings. Go to the `SSH and GPG keys` tab under the *Access* section.

![ssh-setting-github](assets/images/ssh-setting-github.png)

Hit *New SSH key* and paste your *public-key* into the big *Key* text box. Finish by hitting *Add SSH key*.

![add-ssh-key](assets/images/add-ssh-key.png)

4. Test your access

Go back to your terminal and navigate to a folder you want to contain this project. Run the following command to copy the repository to you computer:

```sh
git clone git@github.com:aaatipamula/bool_expr_calc.git
```

### VS Code Access

1. Go to the *Souce Control* tab in VS Code

Hit *Clone Repository* and then *Clone from GitHub* under the command palette. 

![vscode-github-clone](assets/images/vscode-github-clone.png)

2. Link your GitHub account with VS Code

3. Enter `aaatipamula/bool_expr_calc` as the repository you want to clone

Then choose a folder to save the repository in.

![vscode-repository](assets/images/vscode-repository.png)

4. Allow VS Code to sign in using GitHub

[More info](https://code.visualstudio.com/docs/sourcecontrol/github)

## Making/Getting Changes

1. Make a new branch from *main*

> You can name your branch whatever you like but for convention would suggest the format: <firstname>_DD-MM-YY

You can do this in GitHub by going to the [repository](https://github.com/aaatipamula/bool_expr_calc) and hitting the `main` branch button, typing out the name of your new branch, and hitting *Create branch <Your-Branch-Name> from main*. You then have to `git pull` you changes to your local machine.

![github-branch](assets/images/github-branch.png)

You can also just do it on your local machine by swapping branches to *main* with `git checkout main` and then typing out `git branch <Your-Branch-Name>`

2. Make whatever changes you want to the code **within your branch**. (Add and commit them)

3. Push your changes to GitHub with `git push origin <Your-Branch-Name>` or through VS Code.

4. Create a *Pull Request* in GitHub

This can be done by going to the [repository](https://github.com/aaatipamula/bool_expr_calc) and going to the *Pull requests* tab and hitting *New Pull Request*. Choose *main* as the *base branch* and choose *your branch* as the *compare branch*. You may get an error that says something along the lines of "Not able to automatically merge". That's fine just continue making the pull request. 

Someone will then merge it into the main branch and delete your branch you made. To make any more changes restart the process just described.

![github-pr](assets/images/github-pr.png)

