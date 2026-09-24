# Add Your Pong

Want to add your version of Pong to the museum? Good. Apparently we need more Pong.

Each project stays in its own repository. **Pong Museum only stores the gallery entry and preview image**, not the full project source code.

If you do not want to maintain a separate repository, you can also provide us with the source code and we can host it for you.


## What You Need

Before adding your project, make sure you have:

- A public GitHub repository for your Pong project, or the source code if you want us to host it
- A screenshot or GIF showing the game
- A short description
- Your GitHub username or team name
- The language, framework, or libraries you used

## 1. Add Your Screenshot

Place your image in the `res/` directory.

Use a simple file name such as:

```text
images/akil-pong.png
images/alice-multiplayer-pong.gif
```

Prefer PNG for screenshots and GIF only when motion is important.

## 2. Add Your Project to the Gallery

Open `README.md` and add a new project entry under the **Gallery** section.

Use this template:

```md
### Project Name

[![Project Name](res/your-image.png)](https://github.com/OWNER/REPOSITORY)

**By:** [Your GitHub Username](https://github.com/USERNAME)

A short description of your Pong project and what makes it different.

**Built with:** C++, raylib

[View Repository →](https://github.com/OWNER/REPOSITORY)

---
```

Replace all placeholder text with your own project information.

If your project is hosted directly in Pong Museum instead of a separate repository, replace the repository link with the path to your project folder.

## 3. Submit Your Changes

Create a branch for your addition:

```sh
git checkout -b add-yourname-pong
```

Add your screenshot and README changes:

```sh
git add README.md images/
git commit -m "Add YourName's Pong"
git push -u origin add-yourname-pong
```

Then open a Pull Request into the main branch.

## Guidelines

Keep entries short and easy to browse.

- One project entry per Pong version
- Use your own screenshot or GIF
- Link directly to the project repository
- Keep descriptions to a few sentences
- Mention the main language and libraries used
- Do not commit build directories, executables, dependency folders, or the full project source into Pong Museum
- If your project is still unfinished, mark it clearly as a work in progress

There is no requirement for your Pong to be complicated, impressive, or even particularly sensible.

It just needs to be yours.
