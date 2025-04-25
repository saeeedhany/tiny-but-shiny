# 🖌️ Paint

> A minimal drawing app built with SDL — tiny but expressive!

## 📜 What is it?

A basic paint tool that lets you draw with your mouse using different colors and brush sizes.  
Built in pure C with SDL2.

## 🤔 Why did I build it?

I wanted to explore SDL and make something visual and fun.  
This project helped me learn how rendering, events, and color buffers work in practice.

## 🧠 How does it work?

- Initializes SDL and sets up a window and renderer.
- Listens to mouse events to draw rectangles on the screen.
- Uses a color palette and switches colors with keyboard keys.
- Draws onto a persistent canvas using a software loop.

## 🔨 How to build and run

1. Install SDL2:

```bash
sudo apt install libsdl2-dev
```

2. Build:

```bash
cd paint
make
```

3. Run:

```bash
./paint
```

