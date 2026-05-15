# CSS — Reference Manual

> A practical reference for modern CSS (CSS3 and beyond).  
> Links to W3Schools are provided throughout — click them to go deeper on any topic.  
> 📖 Full property index: https://www.w3schools.com/cssref/index.php

---

## 🇬🇧 English

### Table of Contents

1. [How CSS Works — The Basics](#1-how-css-works--the-basics)
2. [Selectors](#2-selectors)
3. [The Cascade, Specificity and Inheritance](#3-the-cascade-specificity-and-inheritance)
4. [Units](#4-units)
5. [Colors](#5-colors)
6. [The Box Model](#6-the-box-model)
7. [Typography and Fonts](#7-typography-and-fonts)
8. [Backgrounds](#8-backgrounds)
9. [Borders and Outlines](#9-borders-and-outlines)
10. [Display and Visibility](#10-display-and-visibility)
11. [Flexbox](#11-flexbox)
12. [CSS Grid](#12-css-grid)
13. [Positioning](#13-positioning)
14. [Sizing](#14-sizing)
15. [Overflow](#15-overflow)
16. [Transforms](#16-transforms)
17. [Transitions](#17-transitions)
18. [Animations and Keyframes](#18-animations-and-keyframes)
19. [Pseudo-classes](#19-pseudo-classes)
20. [Pseudo-elements](#20-pseudo-elements)
21. [Custom Properties — CSS Variables](#21-custom-properties--css-variables)
22. [CSS Functions](#22-css-functions)
23. [At-Rules](#23-at-rules)
24. [Media Queries and Responsive Design](#24-media-queries-and-responsive-design)
25. [Quick Reference Card](#25-quick-reference-card)

---

## 1. How CSS Works — The Basics

CSS (Cascading Style Sheets) describes **how HTML elements should be displayed**. Every CSS rule follows this structure:

```css
selector {
    property: value;
    property: value;
}
```

### Three ways to apply CSS

```html
<!-- 1. External stylesheet — always prefer this -->
<link rel="stylesheet" href="styles.css">

<!-- 2. Internal style block -->
<style>
    p { color: red; }
</style>

<!-- 3. Inline style — last resort, hard to maintain -->
<p style="color: red;">Text</p>
```

> **Rule**: always use an external stylesheet. Inline styles make maintenance a nightmare and are almost impossible to override cleanly.

📖 https://www.w3schools.com/css/css_howto.asp

---

## 2. Selectors

Selectors identify which HTML elements a rule applies to.

📖 https://www.w3schools.com/cssref/css_selectors.php

### Basic selectors

```css
/* Element selector — targets all <p> elements */
p {
    color: #333;
}

/* Class selector — targets any element with class="card" */
.card {
    border-radius: 8px;
}

/* ID selector — targets the unique element with id="header" */
#header {
    background-color: #1a1a2e;
}

/* Universal selector — targets everything */
* {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
}

/* Attribute selector */
a[href] { color: blue; }                      /* has the href attribute */
a[href="https://example.com"] { color: red; } /* exact value */
a[href^="https"] { color: green; }            /* starts with */
a[href$=".pdf"] { color: orange; }            /* ends with */
a[href*="example"] { color: purple; }         /* contains */
input[type="text"] { border: 1px solid #ccc; }
```

📖 https://www.w3schools.com/cssref/css_selectors.php

### Combinators

```css
/* Descendant — any <p> inside .article (any depth) */
.article p { font-size: 1rem; }

/* Child — direct children only */
.nav > li { display: inline-block; }

/* Adjacent sibling — <p> immediately after <h2> */
h2 + p { margin-top: 0; }

/* General sibling — all <p> after <h2> in the same parent */
h2 ~ p { color: #555; }
```

📖 https://www.w3schools.com/css/css_combinators.asp

### Grouping

```css
/* Apply same rules to multiple selectors */
h1, h2, h3, h4 {
    font-family: Georgia, serif;
    font-weight: bold;
}

.btn-primary,
.btn-secondary {
    padding: 8px 16px;
    border-radius: 4px;
    cursor: pointer;
}
```

### Chaining

```css
/* Element AND class — no space between */
p.intro { font-size: 1.2rem; }     /* <p class="intro"> */
a.active { color: orange; }        /* <a class="active"> */

/* Multiple classes */
.card.featured { border: 2px solid gold; }  /* has both classes */
```

---

## 3. The Cascade, Specificity and Inheritance

Understanding these three concepts is what separates CSS that works from CSS that fights you.

📖 https://www.w3schools.com/css/css_specificity.asp

### The Cascade

When multiple rules match the same element, CSS applies them in this order (later wins):

1. Browser default stylesheet
2. External stylesheets (in `<link>` order)
3. Internal `<style>` blocks
4. Inline `style=""` attributes
5. `!important` declarations (overrides everything — avoid)

### Specificity

Each selector has a specificity score. Higher score wins, regardless of order.

| Selector type | Score |
|---|---|
| `style=""` (inline) | 1-0-0-0 |
| `#id` | 0-1-0-0 |
| `.class`, `[attr]`, `:pseudo-class` | 0-0-1-0 |
| `element`, `::pseudo-element` | 0-0-0-1 |
| `*` (universal) | 0-0-0-0 |

```css
/* Specificity: 0-0-0-1 */
p { color: black; }

/* Specificity: 0-0-1-0 — wins over element */
.intro { color: blue; }

/* Specificity: 0-1-0-0 — wins over class */
#hero { color: green; }

/* !important — nuclear option. Avoid it. */
p { color: red !important; }  /* wins over everything */
```

> If you find yourself writing `!important`, the real problem is your selector structure. Fix that instead.

### Inheritance

Some properties are inherited by children, others are not.

```css
/* Inherited by default: color, font-*, line-height, visibility, etc. */
body {
    font-family: sans-serif;  /* all text in the page inherits this */
    color: #333;
}

/* Not inherited by default: margin, padding, border, background, width, etc. */

/* Force inheritance */
.child {
    border: inherit;      /* inherit from parent */
    color: inherit;       /* redundant (already inherited) but explicit */
}

/* Reset to default browser value */
.reset {
    font-size: initial;
}

/* Revert to browser stylesheet */
.revert {
    font-size: revert;
}
```

📖 https://www.w3schools.com/css/css_inherit.asp

---

## 4. Units

📖 https://www.w3schools.com/cssref/css_units.php

### Absolute units

| Unit | Description | Use when |
|---|---|---|
| `px` | Pixels (1/96 of an inch on screen) | Borders, shadows, fixed sizes |
| `pt` | Points (1/72 of an inch) | Print stylesheets |
| `cm`, `mm`, `in` | Physical units | Print only |

### Relative units

| Unit | Relative to | Use when |
|---|---|---|
| `em` | Parent element's `font-size` | Spacing related to local text size |
| `rem` | Root (`<html>`) `font-size` | Consistent spacing across the page |
| `%` | Parent element's value | Widths, heights, positioning |
| `vw` | 1% of viewport width | Full-width layouts |
| `vh` | 1% of viewport height | Full-height sections |
| `vmin` | 1% of smaller viewport dimension | Responsive type |
| `vmax` | 1% of larger viewport dimension | — |
| `ch` | Width of the `0` character | Line length control |
| `ex` | Height of the `x` character | Rarely used |
| `lh` | Current line-height | Vertical rhythm |
| `dvh` | Dynamic viewport height | Mobile (accounts for browser chrome) |

```css
/* Practical examples */
html { font-size: 16px; }   /* base: 1rem = 16px */

h1 { font-size: 2rem; }     /* 32px — scales if html font-size changes */
p  { font-size: 1rem; }     /* 16px */

.container {
    max-width: 1200px;
    width: 90%;              /* fluid until max */
    padding: 0 1rem;
}

.hero {
    height: 100vh;           /* full viewport height */
    min-height: 600px;       /* but never less than 600px */
}

.text {
    max-width: 65ch;         /* no more than 65 characters per line */
    line-height: 1.5;        /* unitless — relative to element's own font-size */
}
```

---

## 5. Colors

📖 https://www.w3schools.com/css/css_colors.asp  
📖 https://www.w3schools.com/cssref/css_colors_legal.php

```css
/* Named colors */
color: red;
color: cornflowerblue;
color: transparent;

/* Hexadecimal — #RRGGBB */
color: #ff0000;       /* red */
color: #333333;       /* dark grey */
color: #333;          /* shorthand — same as #333333 */
color: #ff000088;     /* #RRGGBBAA — with alpha */

/* RGB */
color: rgb(255, 0, 0);
color: rgb(255 0 0);          /* modern syntax — no commas */
color: rgb(255 0 0 / 50%);   /* with alpha */
color: rgba(255, 0, 0, 0.5); /* legacy alpha syntax */

/* HSL — Hue, Saturation, Lightness */
color: hsl(0, 100%, 50%);           /* red */
color: hsl(120, 100%, 50%);         /* green */
color: hsl(240, 100%, 50%);         /* blue */
color: hsl(0 100% 50% / 50%);       /* modern + alpha */
color: hsla(0, 100%, 50%, 0.5);     /* legacy alpha */

/* HWB — Hue, Whiteness, Blackness (CSS Color 4) */
color: hwb(0 0% 0%);         /* red */

/* LCH / OKLCH — perceptually uniform (modern, excellent for gradients) */
color: oklch(0.63 0.26 29);  /* vivid orange */
color: lch(50% 80 30);

/* OKLCH is the best choice for design systems in 2024 */

/* currentColor — inherits the current text color */
.icon {
    fill: currentColor;     /* SVG fill follows the CSS color */
    border-color: currentColor;
}
```

---

## 6. The Box Model

Every element is a rectangular box. Understanding this is non-negotiable.

📖 https://www.w3schools.com/css/css_boxmodel.asp

```
┌─────────────────────────────────┐
│           MARGIN                │  ← transparent, outside
│  ┌───────────────────────────┐  │
│  │         BORDER            │  │
│  │  ┌─────────────────────┐  │  │
│  │  │      PADDING        │  │  │
│  │  │  ┌───────────────┐  │  │  │
│  │  │  │    CONTENT    │  │  │  │
│  │  │  └───────────────┘  │  │  │
│  │  └─────────────────────┘  │  │
│  └───────────────────────────┘  │
└─────────────────────────────────┘
```

```css
.box {
    /* Content size */
    width: 300px;
    height: 200px;

    /* Padding — space between content and border */
    padding: 16px;               /* all sides */
    padding: 8px 16px;           /* top/bottom left/right */
    padding: 4px 8px 12px 16px;  /* top right bottom left (clockwise) */
    padding-top: 8px;
    padding-right: 16px;
    padding-bottom: 8px;
    padding-left: 16px;

    /* Border */
    border: 2px solid #333;
    border-top: 1px dashed red;

    /* Margin — space outside the border */
    margin: 24px;
    margin: 16px auto;           /* auto horizontally centers block elements */
    margin-bottom: 8px;
}

/* box-sizing — the most important reset in CSS */
*, *::before, *::after {
    box-sizing: border-box;
    /*
      content-box (default): width/height = content only
        total = width + padding + border
      border-box:            width/height INCLUDES padding and border
        total = width (padding and border inside)

      Always use border-box. It is how humans think about sizes.
    */
}
```

📖 https://www.w3schools.com/cssref/pr_box-sizing.php  
📖 https://www.w3schools.com/css/css_margin.asp  
📖 https://www.w3schools.com/css/css_padding.asp

### Margin collapse

```css
/*
  Vertical margins between elements COLLAPSE — they don't add up.
  The larger margin wins.

  h1 margin-bottom: 24px
  p  margin-top:    16px
  → actual gap between them: 24px (not 40px)

  This only happens with VERTICAL margins between BLOCK elements.
  Margins inside flex/grid containers do NOT collapse.
*/
```

📖 https://www.w3schools.com/css/css_margin_collapse.asp

---

## 7. Typography and Fonts

📖 https://www.w3schools.com/css/css_font.asp  
📖 https://www.w3schools.com/cssref/pr_font_font-family.php

```css
/* Font family — always list fallbacks */
body {
    font-family: "Inter", "Segoe UI", Arial, sans-serif;
    /*
      Try "Inter" first, fall back to system fonts
      Last value is a generic: serif, sans-serif, monospace, cursive, fantasy
    */
}

/* Font size */
p { font-size: 1rem; }       /* relative to html — preferred */
h1 { font-size: clamp(1.5rem, 4vw, 3rem); }  /* fluid: min, preferred, max */

/* Font weight */
p  { font-weight: 400; }     /* normal */
b  { font-weight: 700; }     /* bold */
/* Valid values: 100 200 300 400 500 600 700 800 900 */
/* Named: normal (400), bold (700), lighter, bolder */

/* Font style */
em { font-style: italic; }
.upright { font-style: normal; }
.oblique { font-style: oblique 15deg; }

/* Line height */
p { line-height: 1.5; }      /* unitless — preferred. 1.5 × font-size */
p { line-height: 150%; }     /* same, but does NOT update for nested elements */

/* Letter spacing */
.headline { letter-spacing: -0.02em; }  /* tighten headlines */
.uppercase { letter-spacing: 0.1em; }  /* loosen spaced caps */

/* Text transformation */
.title  { text-transform: uppercase; }
.name   { text-transform: capitalize; }
.reset  { text-transform: none; }

/* Text alignment */
p       { text-align: left; }
.center { text-align: center; }
.right  { text-align: right; }
.justify { text-align: justify; }

/* Text decoration */
a { text-decoration: none; }          /* remove underline */
.strikethrough { text-decoration: line-through; }
.underline { text-decoration: underline dotted red; }

/* Text overflow */
.truncate {
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;   /* shows "..." when text is clipped */
}

/* Multi-line truncation (modern) */
.clamp {
    display: -webkit-box;
    -webkit-line-clamp: 3;         /* show max 3 lines */
    -webkit-box-orient: vertical;
    overflow: hidden;
}

/* Word spacing and breaking */
p { word-spacing: 0.1em; }
.break-all  { word-break: break-all; }   /* break anywhere */
.break-word { overflow-wrap: break-word; } /* break only if necessary */

/* Font variant — small caps, numeric features */
.caps { font-variant: small-caps; }
.nums { font-variant-numeric: tabular-nums; } /* fixed-width numbers */

/* Web fonts with @font-face */
@font-face {
    font-family: "MyFont";
    src: url("fonts/myfont.woff2") format("woff2"),
         url("fonts/myfont.woff")  format("woff");
    font-weight: 400;
    font-style: normal;
    font-display: swap; /* show fallback font while loading */
}

/* Google Fonts (import in CSS) */
@import url("https://fonts.googleapis.com/css2?family=Inter:wght@400;700&display=swap");
```

📖 https://www.w3schools.com/cssref/pr_font_font-size.php  
📖 https://www.w3schools.com/cssref/pr_font_weight.php  
📖 https://www.w3schools.com/cssref/pr_text_text-align.php  
📖 https://www.w3schools.com/cssref/pr_text_text-overflow.php

---

## 8. Backgrounds

📖 https://www.w3schools.com/css/css_background.asp

```css
.element {
    /* Background color */
    background-color: #f5f5f5;
    background-color: transparent;

    /* Background image */
    background-image: url("photo.jpg");
    background-image: none;

    /* Repeat */
    background-repeat: no-repeat;    /* don't tile */
    background-repeat: repeat;       /* tile both axes (default) */
    background-repeat: repeat-x;     /* tile horizontally */
    background-repeat: repeat-y;     /* tile vertically */

    /* Position */
    background-position: center;
    background-position: top right;
    background-position: 50% 50%;
    background-position: 20px 40px;

    /* Size */
    background-size: cover;    /* fill — may crop */
    background-size: contain;  /* fit — may show empty space */
    background-size: 100px 200px;
    background-size: 50% auto;

    /* Attachment — does it scroll? */
    background-attachment: scroll;  /* default — moves with page */
    background-attachment: fixed;   /* stays fixed — parallax effect */
    background-attachment: local;   /* scrolls with element's content */

    /* Origin and clip */
    background-origin: border-box;
    background-clip: text;   /* clip bg to text shape — great effect */
    -webkit-background-clip: text;

    /* Shorthand */
    background: #1a1a2e url("bg.jpg") no-repeat center / cover;
}

/* Gradients */
.gradient {
    /* Linear gradient */
    background: linear-gradient(to right, #667eea, #764ba2);
    background: linear-gradient(135deg, #f093fb, #f5576c);
    background: linear-gradient(to bottom, #1a1a2e 0%, #16213e 50%, #0f3460 100%);

    /* Radial gradient */
    background: radial-gradient(circle, #f093fb, #f5576c);
    background: radial-gradient(ellipse at top, #e0c3fc, #8ec5fc);

    /* Conic gradient */
    background: conic-gradient(from 90deg, red, yellow, green, blue, red);

    /* Multiple backgrounds — first is on top */
    background:
        url("overlay.png") no-repeat center / cover,
        linear-gradient(135deg, #667eea, #764ba2);
}
```

📖 https://www.w3schools.com/cssref/pr_background-image.php  
📖 https://www.w3schools.com/css/css3_gradients.asp

---

## 9. Borders and Outlines

📖 https://www.w3schools.com/css/css_border.asp

```css
.element {
    /* Border shorthand: width style color */
    border: 1px solid #ccc;
    border-top: 2px dashed red;
    border-right: none;

    /* Individual properties */
    border-width: 1px 2px 1px 2px;  /* top right bottom left */
    border-style: solid;             /* solid dashed dotted double groove ridge inset outset none hidden */
    border-color: #333;

    /* Border radius — rounded corners */
    border-radius: 4px;             /* all corners */
    border-radius: 50%;             /* perfect circle (on square element) */
    border-radius: 16px 4px;        /* top-left/bottom-right  top-right/bottom-left */
    border-radius: 8px 4px 16px 2px; /* top-left top-right bottom-right bottom-left */

    /* Elliptical radius */
    border-radius: 50% / 30%;       /* horizontal / vertical */
    border-top-left-radius: 20px 40px;
}

/* Outline — like border but OUTSIDE the box, doesn't affect layout */
.focused:focus {
    outline: 2px solid #6c63ff;
    outline-offset: 3px;  /* gap between element edge and outline */
}

/* Never do this — it removes accessibility */
/* *:focus { outline: none; } */
/* Instead, replace the outline with something equally visible */

/* Box shadow — not a border, but often used for depth */
.card {
    box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
    /* offset-x  offset-y  blur  spread  color */

    box-shadow: 0 4px 6px -1px rgba(0,0,0,0.1),
                0 2px 4px -1px rgba(0,0,0,0.06);  /* multiple shadows */

    box-shadow: inset 0 2px 4px rgba(0,0,0,0.1);  /* inset — inner shadow */
}
```

📖 https://www.w3schools.com/cssref/pr_border-radius.php  
📖 https://www.w3schools.com/cssref/pr_box-shadow.php

---

## 10. Display and Visibility

📖 https://www.w3schools.com/cssref/pr_class_display.php

```css
/* display — the most important layout property */
.element {
    display: block;          /* full width, new line */
    display: inline;         /* in flow with text, no width/height */
    display: inline-block;   /* in flow, but accepts width/height */
    display: none;           /* removed from layout — space collapses */
    display: flex;           /* flex container */
    display: inline-flex;    /* inline flex container */
    display: grid;           /* grid container */
    display: inline-grid;    /* inline grid container */
    display: contents;       /* element disappears, children remain */
    display: table;          /* behave like <table> */
    display: list-item;      /* behave like <li> */
}

/* visibility — hide but keep the space */
.hidden { visibility: hidden; }     /* invisible but occupies space */
.shown  { visibility: visible; }
.collapse { visibility: collapse; } /* for table rows — collapses the row */

/* opacity */
.translucent { opacity: 0.5; }  /* 0 = invisible, 1 = fully visible */
/* Unlike visibility:hidden, opacity:0 is still clickable/focusable */

/* Hiding accessibly — visible to screen readers */
.sr-only {
    position: absolute;
    width: 1px;
    height: 1px;
    padding: 0;
    margin: -1px;
    overflow: hidden;
    clip: rect(0, 0, 0, 0);
    white-space: nowrap;
    border: 0;
}
```

---

## 11. Flexbox

Flexbox is for **one-dimensional** layout — a row OR a column.

📖 https://www.w3schools.com/css/css3_flexbox.asp  
📖 https://www.w3schools.com/cssref/pr_align-items.php

### Container properties

```css
.flex-container {
    display: flex;         /* or inline-flex */

    /* Direction */
    flex-direction: row;           /* → (default) */
    flex-direction: row-reverse;   /* ← */
    flex-direction: column;        /* ↓ */
    flex-direction: column-reverse;/* ↑ */

    /* Wrapping */
    flex-wrap: nowrap;    /* single line (default) */
    flex-wrap: wrap;      /* wrap to next line */
    flex-wrap: wrap-reverse;

    /* Shorthand */
    flex-flow: row wrap;

    /* Main axis alignment (justify-content) */
    justify-content: flex-start;    /* pack to start */
    justify-content: flex-end;      /* pack to end */
    justify-content: center;        /* center */
    justify-content: space-between; /* equal space BETWEEN items */
    justify-content: space-around;  /* equal space AROUND items */
    justify-content: space-evenly;  /* equal space everywhere */

    /* Cross axis alignment (align-items) */
    align-items: stretch;      /* fill container height (default) */
    align-items: flex-start;   /* align to start of cross axis */
    align-items: flex-end;     /* align to end */
    align-items: center;       /* center on cross axis */
    align-items: baseline;     /* align text baselines */

    /* Multi-line cross axis (align-content) — only when flex-wrap: wrap */
    align-content: flex-start;
    align-content: center;
    align-content: space-between;

    /* Gap between items */
    gap: 16px;             /* row and column gap */
    gap: 8px 16px;         /* row-gap column-gap */
    row-gap: 8px;
    column-gap: 16px;
}
```

### Item properties

```css
.flex-item {
    /* Grow — how much to grow relative to siblings */
    flex-grow: 0;    /* don't grow (default) */
    flex-grow: 1;    /* grow to fill available space */
    flex-grow: 2;    /* grow twice as much as flex-grow:1 siblings */

    /* Shrink — how much to shrink if needed */
    flex-shrink: 1;  /* shrink if needed (default) */
    flex-shrink: 0;  /* never shrink */

    /* Basis — initial main size before grow/shrink */
    flex-basis: auto;    /* use content size (default) */
    flex-basis: 200px;   /* start at 200px */
    flex-basis: 0;       /* ignore content size, distribute purely by flex-grow */

    /* Shorthand: grow shrink basis */
    flex: 1;             /* flex: 1 1 0 — common: grow, shrink, start from 0 */
    flex: auto;          /* flex: 1 1 auto */
    flex: none;          /* flex: 0 0 auto — rigid */
    flex: 0 0 200px;     /* fixed 200px, no grow, no shrink */

    /* Self alignment — override container's align-items */
    align-self: auto;
    align-self: center;
    align-self: flex-end;
    align-self: stretch;

    /* Order — visual reordering (doesn't affect DOM) */
    order: 0;     /* default */
    order: -1;    /* move to start */
    order: 1;     /* move to end */
}
```

### Common Flexbox patterns

```css
/* Center anything */
.center {
    display: flex;
    justify-content: center;
    align-items: center;
}

/* Navigation bar */
.navbar {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 0 24px;
}

/* Card row that wraps */
.card-grid {
    display: flex;
    flex-wrap: wrap;
    gap: 16px;
}
.card {
    flex: 1 1 300px;  /* grow/shrink freely, min width 300px */
    max-width: 400px;
}

/* Sticky footer */
body {
    display: flex;
    flex-direction: column;
    min-height: 100vh;
}
main { flex: 1; }   /* main pushes footer to bottom */
```

---

## 12. CSS Grid

Grid is for **two-dimensional** layout — rows AND columns.

📖 https://www.w3schools.com/css/css_grid.asp  
📖 https://www.w3schools.com/cssref/pr_grid-template-columns.php

### Container properties

```css
.grid-container {
    display: grid;   /* or inline-grid */

    /* Define columns */
    grid-template-columns: 200px 1fr 1fr;      /* fixed + flexible */
    grid-template-columns: repeat(3, 1fr);     /* 3 equal columns */
    grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));  /* responsive */
    grid-template-columns: repeat(auto-fit,  minmax(250px, 1fr));  /* collapse empty */

    /* Define rows */
    grid-template-rows: auto;
    grid-template-rows: 80px 1fr 60px;         /* header main footer */
    grid-auto-rows: minmax(100px, auto);        /* implicit rows min size */

    /* Named areas */
    grid-template-areas:
        "header  header"
        "sidebar main  "
        "footer  footer";

    /* Gaps */
    gap: 24px;
    row-gap: 16px;
    column-gap: 24px;

    /* Alignment of all items */
    justify-items: start | end | center | stretch; /* horizontal */
    align-items:   start | end | center | stretch; /* vertical */

    /* Alignment of the grid itself in its container */
    justify-content: start | end | center | space-between | space-around;
    align-content:   start | end | center | space-between;
}
```

### Item properties

```css
.grid-item {
    /* Placement by line numbers */
    grid-column: 1 / 3;        /* from line 1 to line 3 */
    grid-column: 1 / -1;       /* from line 1 to last line */
    grid-column: span 2;       /* span 2 columns */
    grid-row:    1 / 3;        /* rows 1 to 3 */
    grid-row:    span 2;

    /* Shorthand: row-start / col-start / row-end / col-end */
    grid-area: 1 / 2 / 3 / 4;

    /* Placement by named area */
    grid-area: header;
    grid-area: sidebar;
    grid-area: main;
    grid-area: footer;

    /* Self alignment */
    justify-self: start | end | center | stretch;
    align-self:   start | end | center | stretch;
}
```

### Named template areas example

```css
.page {
    display: grid;
    grid-template-columns: 250px 1fr;
    grid-template-rows: 60px 1fr 50px;
    grid-template-areas:
        "header  header"
        "sidebar main  "
        "footer  footer";
    min-height: 100vh;
}

.header  { grid-area: header; }
.sidebar { grid-area: sidebar; }
.main    { grid-area: main; }
.footer  { grid-area: footer; }
```

### Responsive grid (no media queries needed)

```css
.auto-grid {
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
    gap: 24px;
    /*
      auto-fill: creates as many columns as fit
      minmax(280px, 1fr): each column is at least 280px, max flexible
      Result: automatically adjusts columns to screen width
    */
}
```

📖 https://www.w3schools.com/cssref/pr_grid-template-areas.php  
📖 https://www.w3schools.com/cssref/func_minmax.php

---

## 13. Positioning

📖 https://www.w3schools.com/css/css_positioning.asp

```css
.element {
    position: static;    /* default — in normal document flow */
    position: relative;  /* offset from its normal position, still in flow */
    position: absolute;  /* removed from flow, positioned relative to nearest non-static ancestor */
    position: fixed;     /* removed from flow, relative to viewport — stays on scroll */
    position: sticky;    /* hybrid: relative until scroll threshold, then fixed */
}

/* relative — nudge without affecting layout */
.nudge {
    position: relative;
    top: -2px;    /* move 2px up from normal position */
    left: 0;
}

/* absolute — precise placement */
.parent {
    position: relative;  /* establish positioning context for children */
}
.badge {
    position: absolute;
    top: -8px;
    right: -8px;
    /* placed relative to .parent */
}

/* Center with absolute */
.centered {
    position: absolute;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);  /* pull back by half own size */
}

/* fixed — navbar, modal overlay */
.sticky-nav {
    position: fixed;
    top: 0;
    left: 0;
    right: 0;
    z-index: 100;
}

.modal-overlay {
    position: fixed;
    inset: 0;   /* shorthand for top: 0; right: 0; bottom: 0; left: 0 */
    background: rgba(0, 0, 0, 0.5);
}

/* sticky — stays in flow until threshold */
.sticky-header {
    position: sticky;
    top: 0;           /* sticks when scroll reaches top:0 */
    z-index: 10;
}

/* z-index — stacking order */
.overlay  { z-index: 100; }   /* on top */
.modal    { z-index: 200; }   /* above overlay */
.tooltip  { z-index: 300; }   /* above modal */
/* z-index only works on non-static positioned elements */
```

📖 https://www.w3schools.com/cssref/pr_pos_z-index.php

---

## 14. Sizing

📖 https://www.w3schools.com/css/css_dimension.asp

```css
.element {
    /* Width and height */
    width: 300px;
    width: 50%;
    width: auto;        /* default — computed by browser */
    width: fit-content; /* shrink to content */
    width: min-content; /* minimum possible width (longest unbreakable word) */
    width: max-content; /* width without any wrapping */

    height: 200px;
    height: auto;       /* default — determined by content */
    height: 100%;       /* only works if parent has explicit height */
    height: 100vh;      /* full viewport height */

    /* Constraints */
    min-width: 200px;
    max-width: 1200px;
    min-height: 100px;
    max-height: 600px;

    /* Aspect ratio */
    aspect-ratio: 16 / 9;    /* maintain 16:9 ratio */
    aspect-ratio: 1;          /* square */
    aspect-ratio: 4 / 3;

    /* Object fit — for <img> and <video> inside a sized container */
    object-fit: cover;    /* fill, may crop */
    object-fit: contain;  /* fit without cropping */
    object-fit: fill;     /* stretch to fit — distorts */
    object-fit: none;     /* original size */
    object-position: center top;  /* where to crop */
}
```

📖 https://www.w3schools.com/cssref/pr_dim_min-width.php  
📖 https://www.w3schools.com/cssref/pr_object-fit.php

---

## 15. Overflow

📖 https://www.w3schools.com/cssref/pr_pos_overflow.php

```css
.element {
    overflow: visible;  /* content spills out (default) */
    overflow: hidden;   /* clip content */
    overflow: scroll;   /* always show scrollbars */
    overflow: auto;     /* scrollbars only when needed — prefer this */

    overflow-x: auto;   /* horizontal scroll */
    overflow-y: hidden; /* clip vertical */

    /* Modern scroll customization */
    scroll-behavior: smooth;          /* animated scrolling */
    scroll-snap-type: x mandatory;    /* snap to items on scroll */
    overscroll-behavior: contain;     /* prevent scroll chaining to parent */
}

/* Custom scrollbar (webkit browsers) */
.scrollable::-webkit-scrollbar { width: 8px; }
.scrollable::-webkit-scrollbar-track { background: #f1f1f1; }
.scrollable::-webkit-scrollbar-thumb {
    background: #888;
    border-radius: 4px;
}
.scrollable::-webkit-scrollbar-thumb:hover { background: #555; }
```

---

## 16. Transforms

Transforms move, rotate, scale, or skew elements **without affecting layout**.

📖 https://www.w3schools.com/css/css3_2dtransforms.asp  
📖 https://www.w3schools.com/css/css3_3dtransforms.asp

```css
.element {
    /* 2D transforms */
    transform: translateX(50px);
    transform: translateY(-20px);
    transform: translate(50px, -20px);    /* x, y */

    transform: scale(1.5);                /* scale both axes */
    transform: scaleX(2);
    transform: scaleY(0.5);

    transform: rotate(45deg);
    transform: rotate(-90deg);

    transform: skewX(15deg);
    transform: skewY(10deg);
    transform: skew(15deg, 10deg);

    /* 3D transforms */
    transform: translateZ(100px);
    transform: translate3d(50px, 20px, 100px);
    transform: rotateX(45deg);
    transform: rotateY(45deg);
    transform: rotateZ(45deg);  /* same as rotate() */
    transform: perspective(500px) rotateY(45deg);

    /* Chaining — applied right to left */
    transform: translate(50px, 50px) rotate(45deg) scale(1.5);

    /* Transform origin — pivot point */
    transform-origin: center;        /* default */
    transform-origin: top left;
    transform-origin: 50% 100%;
    transform-origin: 0 0;           /* top-left corner */

    /* 3D perspective for parent */
    perspective: 1000px;
    perspective-origin: center;

    /* 3D rendering */
    transform-style: preserve-3d;  /* children in 3D space */
    backface-visibility: hidden;   /* hide the back of a flipped element */
}
```

📖 https://www.w3schools.com/cssref/pr_transform.php

---

## 17. Transitions

Transitions animate changes between two states smoothly.

📖 https://www.w3schools.com/css/css3_transitions.asp

```css
.button {
    background-color: #6c63ff;
    color: white;
    padding: 10px 20px;
    border-radius: 6px;

    /* Transition: property duration timing-function delay */
    transition: background-color 0.3s ease;

    /* Multiple transitions */
    transition:
        background-color 0.3s ease,
        transform 0.15s ease-out,
        box-shadow 0.3s ease;

    /* Transition all properties */
    transition: all 0.3s ease;  /* convenient but can cause performance issues */
}

.button:hover {
    background-color: #5a52d5;
    transform: translateY(-2px);
    box-shadow: 0 4px 12px rgba(108, 99, 255, 0.4);
}

.button:active {
    transform: translateY(0);
}
```

### Timing functions

```css
transition-timing-function: linear;                        /* constant speed */
transition-timing-function: ease;                          /* slow → fast → slow (default) */
transition-timing-function: ease-in;                       /* slow start */
transition-timing-function: ease-out;                      /* slow end */
transition-timing-function: ease-in-out;                   /* slow start and end */
transition-timing-function: cubic-bezier(0.68, -0.55, 0.27, 1.55); /* custom — bouncy */
transition-timing-function: steps(4, end);                 /* stepped — no interpolation */
```

📖 https://www.w3schools.com/cssref/pr_transition-timing-function.php

---

## 18. Animations and Keyframes

Animations run automatically, can loop, and give full control over the timeline.

📖 https://www.w3schools.com/css/css3_animations.asp

```css
/* Define the animation */
@keyframes fadeIn {
    from {
        opacity: 0;
        transform: translateY(20px);
    }
    to {
        opacity: 1;
        transform: translateY(0);
    }
}

@keyframes pulse {
    0%   { transform: scale(1); }
    50%  { transform: scale(1.05); }
    100% { transform: scale(1); }
}

@keyframes spin {
    from { transform: rotate(0deg); }
    to   { transform: rotate(360deg); }
}

@keyframes colorShift {
    0%   { background-color: #6c63ff; }
    33%  { background-color: #f5576c; }
    66%  { background-color: #f093fb; }
    100% { background-color: #6c63ff; }
}

/* Apply the animation */
.fade-in {
    animation-name: fadeIn;
    animation-duration: 0.5s;
    animation-timing-function: ease-out;
    animation-delay: 0.2s;
    animation-iteration-count: 1;         /* number or infinite */
    animation-direction: normal;          /* normal | reverse | alternate | alternate-reverse */
    animation-fill-mode: forwards;        /* keep end state after animation */
    animation-play-state: running;        /* running | paused */

    /* Shorthand: name duration timing delay count direction fill-mode */
    animation: fadeIn 0.5s ease-out 0.2s 1 normal forwards;
}

.loader {
    animation: spin 1s linear infinite;
}

.card:hover {
    animation: pulse 0.6s ease-in-out;
}

/* Pause on hover */
.animated:hover {
    animation-play-state: paused;
}

/* Respect user preference for reduced motion */
@media (prefers-reduced-motion: reduce) {
    * {
        animation-duration: 0.01ms !important;
        animation-iteration-count: 1 !important;
        transition-duration: 0.01ms !important;
    }
}
```

📖 https://www.w3schools.com/cssref/pr_animation.php  
📖 https://www.w3schools.com/cssref/atrule_keyframes.php

---

## 19. Pseudo-classes

Pseudo-classes select elements based on state or position.

📖 https://www.w3schools.com/css/css_pseudo_classes.asp

### User interaction states

```css
a:link    { color: blue; }          /* unvisited link */
a:visited { color: purple; }        /* visited link */
a:hover   { color: darkblue; }      /* mouse over */
a:active  { color: red; }           /* being clicked */
/* Order matters: L-V-H-A (LoVe HAte) */

button:focus         { outline: 2px solid blue; }       /* keyboard focus */
button:focus-visible { outline: 2px solid blue; }       /* focus via keyboard only */
button:focus-within  { background: #f0f0f0; }           /* descendant is focused */

input:enabled  { background: white; }
input:disabled { background: #ccc; cursor: not-allowed; }
input:checked  { accent-color: #6c63ff; }
input:required { border-color: red; }
input:valid    { border-color: green; }
input:invalid  { border-color: red; }
input:placeholder-shown { border-color: #ccc; }
input:in-range { border-color: green; }
input:out-of-range { border-color: red; }
```

### Structural pseudo-classes

```css
/* Child selectors */
li:first-child       { font-weight: bold; }
li:last-child        { border-bottom: none; }
li:nth-child(2)      { color: red; }          /* 2nd child */
li:nth-child(odd)    { background: #f9f9f9; } /* odd rows */
li:nth-child(even)   { background: white; }
li:nth-child(3n)     { color: green; }        /* every 3rd */
li:nth-child(3n+1)   { color: blue; }         /* 1st, 4th, 7th... */
li:nth-last-child(2) { color: orange; }       /* 2nd from last */
li:only-child        { margin: auto; }        /* only child */

/* Type selectors */
p:first-of-type  { font-size: 1.2em; }       /* first <p> in its parent */
p:last-of-type   { margin-bottom: 0; }
p:nth-of-type(2) { color: blue; }
p:only-of-type   { text-align: center; }

/* Empty / not */
div:empty { display: none; }                  /* no children or text */
p:not(.intro) { color: #555; }               /* exclude .intro */
p:not(:first-child) { margin-top: 0; }
:not(p, li) { list-style: none; }            /* multiple :not() — CSS4 */

/* Target */
:target { background: yellow; }              /* element matching URL #hash */

/* Scope */
:root { --primary: #6c63ff; }               /* the html element */

/* Has — parent selector (CSS4) */
.card:has(img) { padding: 0; }              /* card that contains an img */
form:has(:invalid) { border-color: red; }   /* form with invalid input */
```

📖 https://www.w3schools.com/cssref/sel_nth-child.php  
📖 https://www.w3schools.com/cssref/sel_not.php  
📖 https://www.w3schools.com/cssref/sel_has.php

---

## 20. Pseudo-elements

Pseudo-elements create virtual elements that don't exist in the HTML.

📖 https://www.w3schools.com/css/css_pseudo_elements.asp

```css
/* ::before and ::after — insert content before/after the element */
.button::before {
    content: "→ ";         /* required, even if empty: content: "" */
    color: currentColor;
}

.required::after {
    content: " *";
    color: red;
}

/* Create visual decoration with empty content */
.divider::before {
    content: "";
    display: block;
    height: 2px;
    background: linear-gradient(to right, transparent, #6c63ff, transparent);
}

/* ::first-line — style first line of text */
p::first-line {
    font-weight: bold;
    font-size: 1.1em;
}

/* ::first-letter — style first character (drop cap) */
p::first-letter {
    font-size: 3em;
    float: left;
    line-height: 1;
    margin-right: 4px;
    color: #6c63ff;
}

/* ::placeholder — style input placeholder text */
input::placeholder {
    color: #999;
    font-style: italic;
}

/* ::selection — style selected text */
::selection {
    background: #6c63ff;
    color: white;
}

/* ::marker — style list markers (bullets, numbers) */
li::marker {
    color: #6c63ff;
    font-size: 1.2em;
}

/* ::backdrop — behind a <dialog> or fullscreen element */
dialog::backdrop {
    background: rgba(0, 0, 0, 0.6);
    backdrop-filter: blur(4px);
}
```

📖 https://www.w3schools.com/cssref/sel_before.php  
📖 https://www.w3schools.com/cssref/sel_selection.php

---

## 21. Custom Properties — CSS Variables

📖 https://www.w3schools.com/css/css3_variables.asp

```css
/* Define — typically on :root for global scope */
:root {
    /* Colors */
    --color-primary:   #6c63ff;
    --color-secondary: #f5576c;
    --color-text:      #1a1a2e;
    --color-muted:     #6b7280;
    --color-bg:        #ffffff;
    --color-surface:   #f9fafb;

    /* Typography */
    --font-sans: "Inter", system-ui, sans-serif;
    --font-mono: "JetBrains Mono", monospace;
    --text-sm:   0.875rem;
    --text-base: 1rem;
    --text-lg:   1.125rem;
    --text-xl:   1.25rem;
    --text-2xl:  1.5rem;
    --text-3xl:  1.875rem;

    /* Spacing scale */
    --space-1: 0.25rem;   /*  4px */
    --space-2: 0.5rem;    /*  8px */
    --space-3: 0.75rem;   /* 12px */
    --space-4: 1rem;      /* 16px */
    --space-6: 1.5rem;    /* 24px */
    --space-8: 2rem;      /* 32px */

    /* Borders */
    --radius-sm: 4px;
    --radius-md: 8px;
    --radius-lg: 16px;
    --radius-full: 9999px;

    /* Shadows */
    --shadow-sm: 0 1px 2px rgba(0,0,0,0.05);
    --shadow-md: 0 4px 6px -1px rgba(0,0,0,0.1);
    --shadow-lg: 0 10px 15px -3px rgba(0,0,0,0.1);

    /* Transitions */
    --duration-fast:   150ms;
    --duration-normal: 300ms;
    --ease-out: cubic-bezier(0, 0, 0.2, 1);
}

/* Use */
.button {
    background-color: var(--color-primary);
    padding: var(--space-2) var(--space-4);
    border-radius: var(--radius-md);
    font-family: var(--font-sans);
    transition: background-color var(--duration-fast) var(--ease-out);
}

/* Fallback value */
.element {
    color: var(--color-accent, #6c63ff);  /* use #6c63ff if --color-accent is not defined */
}

/* Override at component level */
.card {
    --color-primary: #f5576c;  /* local override — only affects descendants */
}

/* Dark mode with variables */
@media (prefers-color-scheme: dark) {
    :root {
        --color-text: #f9fafb;
        --color-bg:   #1a1a2e;
        --color-surface: #16213e;
    }
}

/* Use in calculations */
:root { --base-size: 4px; }
.element {
    padding: calc(var(--base-size) * 4);  /* 16px */
    margin:  calc(var(--base-size) * 6);  /* 24px */
}
```

---

## 22. CSS Functions

📖 https://www.w3schools.com/cssref/css_functions.php

```css
/* calc() — arithmetic with mixed units */
.element {
    width: calc(100% - 48px);
    height: calc(100vh - 80px);
    padding: calc(1rem + 4px);
    font-size: calc(16px + 0.5vw);
}

/* min() — smallest of values */
.container {
    width: min(100%, 1200px);     /* 100% or 1200px, whichever is smaller */
}

/* max() — largest of values */
.content {
    padding: max(16px, 4vw);     /* at least 16px */
}

/* clamp(min, preferred, max) — responsive range */
h1 {
    font-size: clamp(1.5rem, 4vw, 3rem);  /* never below 1.5rem, never above 3rem */
}
.container {
    width: clamp(300px, 80%, 1200px);
}

/* var() — CSS custom properties */
color: var(--color-primary, #6c63ff);

/* rgb() / hsl() / oklch() — colors */
color: rgb(108 99 255 / 80%);
color: hsl(248 100% 70%);
color: oklch(0.65 0.2 270);

/* url() — load a resource */
background-image: url("bg.jpg");
cursor: url("cursor.png"), auto;

/* linear-gradient() / radial-gradient() / conic-gradient() */
background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
background: radial-gradient(circle at top, #f093fb, transparent);

/* counter() — CSS counters */
.numbered-list li::before {
    content: counter(item) ". ";
    counter-increment: item;
}

/* attr() — use HTML attribute as value */
a::after { content: " (" attr(href) ")"; }  /* show href in print */

/* env() — environment variables (safe area for notched phones) */
.content {
    padding-bottom: env(safe-area-inset-bottom);
}

/* fit-content(), min-content, max-content */
.element {
    width: fit-content;
    grid-template-columns: min-content 1fr;
}

/* repeat() — grid shorthand */
grid-template-columns: repeat(3, 1fr);
grid-template-columns: repeat(auto-fill, minmax(200px, 1fr));

/* minmax() — grid range */
grid-auto-rows: minmax(100px, auto);

/* translate(), rotate(), scale() — individual transform functions (CSS Level 5) */
.element {
    translate: 10px 20px;   /* instead of transform: translate() */
    rotate: 45deg;
    scale: 1.5;
}
```

---

## 23. At-Rules

📖 https://www.w3schools.com/cssref/atrule_media.php

```css
/* @import — load another CSS file */
@import url("variables.css");
@import url("reset.css") layer;
@import url("theme.css") supports(display: grid);

/* @font-face — custom web fonts */
@font-face {
    font-family: "MyFont";
    src: url("font.woff2") format("woff2");
    font-weight: 100 900;      /* variable font weight range */
    font-style: normal;
    font-display: swap;
}

/* @media — responsive design (see section 24) */
@media (max-width: 768px) { ... }

/* @keyframes — animation definition (see section 18) */
@keyframes name { ... }

/* @layer — cascade layers (modern, very useful for design systems) */
@layer reset, base, components, utilities;

@layer reset {
    * { margin: 0; padding: 0; box-sizing: border-box; }
}
@layer base {
    body { font-family: sans-serif; }
}
@layer components {
    .button { padding: 8px 16px; }
}
@layer utilities {
    .text-center { text-align: center; }
}
/* Rules in later layers win over earlier layers, regardless of specificity */

/* @supports — feature detection */
@supports (display: grid) {
    .container { display: grid; }
}
@supports not (display: grid) {
    .container { display: flex; }
}
@supports (backdrop-filter: blur(10px)) {
    .glass { backdrop-filter: blur(10px); }
}

/* @container — container queries (modern — very powerful) */
.card-wrapper {
    container-type: inline-size;
    container-name: card;
}
@container card (min-width: 400px) {
    .card { flex-direction: row; }
}
@container card (max-width: 400px) {
    .card { flex-direction: column; }
}

/* @property — define typed custom properties (Houdini) */
@property --rotation {
    syntax: "<angle>";
    inherits: false;
    initial-value: 0deg;
}

/* @page — print layout */
@page {
    size: A4;
    margin: 2cm;
}
@page :first { margin-top: 4cm; }

/* @charset — character encoding (must be first line) */
@charset "UTF-8";
```

📖 https://www.w3schools.com/cssref/atrule_font-face.php  
📖 https://www.w3schools.com/cssref/atrule_supports.php

---

## 24. Media Queries and Responsive Design

📖 https://www.w3schools.com/css/css_rwd_mediaqueries.asp  
📖 https://www.w3schools.com/cssref/atrule_media.php

```css
/* Syntax */
@media media-type and (condition) { ... }

/* Media types */
@media screen  { ... }  /* screens */
@media print   { ... }  /* print */
@media all     { ... }  /* all (default) */

/* Common breakpoints (mobile-first approach) */
/* Base styles — for all screens (mobile first) */
.container { padding: 16px; }

/* Small tablets and up */
@media (min-width: 640px) {
    .container { padding: 24px; }
}

/* Tablets and up */
@media (min-width: 768px) {
    .container { padding: 32px; }
    .grid { grid-template-columns: 1fr 1fr; }
}

/* Desktops and up */
@media (min-width: 1024px) {
    .container { max-width: 1024px; margin: auto; }
    .grid { grid-template-columns: repeat(3, 1fr); }
}

/* Large desktops */
@media (min-width: 1280px) {
    .container { max-width: 1280px; }
}

/* Range syntax (modern — much cleaner) */
@media (width >= 768px)  { ... }
@media (width <= 1024px) { ... }
@media (768px <= width <= 1024px) { ... }   /* between */

/* Combining conditions */
@media (min-width: 768px) and (max-width: 1024px) { ... }  /* classic */
@media (min-width: 768px) and (orientation: landscape) { ... }

/* Other conditions */
@media (orientation: portrait)  { ... }
@media (orientation: landscape) { ... }
@media (hover: hover)           { ... }  /* device supports hover */
@media (hover: none)            { ... }  /* touch device */
@media (pointer: fine)          { ... }  /* mouse */
@media (pointer: coarse)        { ... }  /* touch */
@media (prefers-color-scheme: dark)  { ... }  /* dark mode */
@media (prefers-color-scheme: light) { ... }
@media (prefers-reduced-motion: reduce) { ... }  /* user wants less motion */
@media (prefers-contrast: more) { ... }          /* high contrast preference */
@media (display-mode: standalone) { ... }        /* installed PWA */
@media print { ... }                             /* print stylesheet */

/* Logical operators */
@media not screen { ... }
@media screen and (min-width: 768px) { ... }
@media (min-width: 600px) or (orientation: landscape) { ... }
```

### Responsive patterns

```css
/* Fluid type — text scales between breakpoints */
html {
    font-size: clamp(14px, 1.5vw, 18px);
}

/* Responsive images */
img {
    max-width: 100%;
    height: auto;
}

/* Hide on mobile */
.desktop-only {
    display: none;
}
@media (min-width: 768px) {
    .desktop-only { display: block; }
    .mobile-only  { display: none; }
}

/* Fluid container */
.container {
    width: min(100% - 32px, 1200px);
    margin-inline: auto;  /* logical property: left + right margin */
}
```

📖 https://www.w3schools.com/css/css_rwd_intro.asp

---

## 25. Quick Reference Card

### Selector cheat sheet

| Selector | Selects |
|---|---|
| `*` | All elements |
| `div` | All `<div>` |
| `.class` | Elements with class |
| `#id` | Element with id |
| `div p` | `<p>` inside `<div>` (any depth) |
| `div > p` | Direct `<p>` children of `<div>` |
| `h2 + p` | `<p>` immediately after `<h2>` |
| `h2 ~ p` | All `<p>` after `<h2>` in same parent |
| `[attr]` | Has attribute |
| `[attr="val"]` | Attribute equals value |
| `[attr^="val"]` | Attribute starts with |
| `[attr$="val"]` | Attribute ends with |
| `[attr*="val"]` | Attribute contains |
| `:hover` | Mouse over |
| `:focus` | Keyboard focused |
| `:nth-child(n)` | nth child |
| `:not(x)` | Does not match x |
| `:has(x)` | Contains x |
| `::before` | Virtual element before content |
| `::after` | Virtual element after content |

### Specificity cheat sheet

```
Inline style           → 1,0,0,0
#id                    → 0,1,0,0
.class / [attr] / :pseudo-class → 0,0,1,0
element / ::pseudo-element      → 0,0,0,1
* combinator           → 0,0,0,0

Higher number wins. Same number → last in source wins.
!important overrides everything — use it as a last resort.
```

### Box model cheat sheet

```css
/* Use this reset globally */
*, *::before, *::after { box-sizing: border-box; }

/* Shorthand order: Top Right Bottom Left (clockwise) */
margin: T R B L;
padding: T R B L;
border-width: T R B L;

/* Two values: T/B  R/L */
margin: 16px 24px;

/* One value: all sides */
padding: 16px;
```

### Flexbox cheat sheet

```
Container:               Item:
display: flex            flex: 1          (grow)
flex-direction: row      flex: none       (rigid)
flex-wrap: wrap          flex: 0 0 200px  (fixed)
justify-content:         align-self:
  flex-start               flex-start
  flex-end                 flex-end
  center                   center
  space-between            stretch
  space-around           order: -1 / 0 / 1
  space-evenly
align-items:
  stretch
  flex-start
  flex-end
  center
  baseline
gap: 16px
```

### Grid cheat sheet

```
Container:                          Item:
display: grid                       grid-column: 1 / 3
grid-template-columns:              grid-column: span 2
  repeat(3, 1fr)                    grid-row: 1 / -1
  repeat(auto-fill, minmax(250px, 1fr))  grid-area: header
grid-template-rows: auto
grid-template-areas: "..."
gap: 16px
justify-items: stretch
align-items: stretch
```

### Frequently confused things

| | |
|---|---|
| `margin` vs `padding` | margin = outside (transparent), padding = inside |
| `width: 100%` | relative to parent — parent must have a defined width |
| `height: 100%` | only works if parent has an explicit height |
| `position: absolute` | relative to nearest non-static ancestor |
| `z-index` | only works on non-static elements |
| `display: none` | removes from layout completely |
| `visibility: hidden` | invisible but occupies space |
| `opacity: 0` | invisible but occupies space AND is still clickable |
| `em` | relative to current element's font-size |
| `rem` | relative to root `<html>` font-size |
| `transition` | animate between TWO states (before/after) |
| `animation` | animate along a TIMELINE (keyframes), can loop |
| `:hover` | user interaction pseudo-class |
| `::before` | generated content pseudo-element |

---

---

## 🇫🇷 Français

### Table des matières

1. [Comment fonctionne CSS](#1-comment-fonctionne-css)
2. [Sélecteurs](#2-sélecteurs)
3. [La cascade, la spécificité et l'héritage](#3-la-cascade-la-spécificité-et-lhéritage)
4. [Unités](#4-unités-1)
5. [Couleurs](#5-couleurs)
6. [Le modèle de boîte](#6-le-modèle-de-boîte)
7. [Typographie et polices](#7-typographie-et-polices)
8. [Arrière-plans](#8-arrière-plans)
9. [Bordures et contours](#9-bordures-et-contours)
10. [Display et visibilité](#10-display-et-visibilité)
11. [Flexbox](#11-flexbox-1)
12. [CSS Grid](#12-css-grid-1)
13. [Positionnement](#13-positionnement)
14. [Dimensionnement](#14-dimensionnement)
15. [Overflow](#15-overflow-1)
16. [Transformations](#16-transformations)
17. [Transitions](#17-transitions-1)
18. [Animations et keyframes](#18-animations-et-keyframes)
19. [Pseudo-classes](#19-pseudo-classes-1)
20. [Pseudo-éléments](#20-pseudo-éléments)
21. [Propriétés personnalisées — Variables CSS](#21-propriétés-personnalisées--variables-css)
22. [Fonctions CSS](#22-fonctions-css)
23. [Règles @](#23-règles-)
24. [Media Queries et design responsive](#24-media-queries-et-design-responsive)
25. [Carte de référence rapide](#25-carte-de-référence-rapide-1)

---

## 1. Comment fonctionne CSS

CSS (Cascading Style Sheets — feuilles de style en cascade) décrit **comment les éléments HTML doivent être affichés**. Chaque règle CSS suit cette structure :

```css
sélecteur {
    propriété: valeur;
    propriété: valeur;
}
```

### Trois façons d'appliquer CSS

```html
<!-- 1. Feuille de style externe — toujours préférer -->
<link rel="stylesheet" href="styles.css">

<!-- 2. Bloc de style interne -->
<style>
    p { color: red; }
</style>

<!-- 3. Style inline — en dernier recours, difficile à maintenir -->
<p style="color: red;">Texte</p>
```

> **Règle** : utilise toujours une feuille de style externe. Les styles inline rendent la maintenance cauchemardesque et sont presque impossibles à surcharger proprement.

📖 https://www.w3schools.com/css/css_howto.asp

---

## 2. Sélecteurs

```css
/* Élément — cible tous les <p> */
p { color: #333; }

/* Classe — cible tout élément avec class="carte" */
.carte { border-radius: 8px; }

/* ID — cible l'élément unique avec id="entete" */
#entete { background-color: #1a1a2e; }

/* Universel */
* { box-sizing: border-box; }

/* Attribut */
a[href^="https"] { color: green; }     /* commence par */
a[href$=".pdf"]  { color: orange; }    /* se termine par */
a[href*="example"] { color: purple; }  /* contient */

/* Combinateurs */
.article p  { /* descendant (toute profondeur) */ }
.nav > li   { /* enfant direct */ }
h2 + p      { /* frère adjacent */ }
h2 ~ p      { /* tous les frères suivants */ }

/* Chaînage */
p.intro      { /* <p class="intro"> */ }
.card.active { /* possède les deux classes */ }
```

📖 https://www.w3schools.com/css/css_combinators.asp  
📖 https://www.w3schools.com/cssref/css_selectors.php

---

## 3. La cascade, la spécificité et l'héritage

Comprendre ces trois concepts distingue le CSS qui fonctionne de celui qui se bat contre toi.

### La spécificité

| Type de sélecteur | Score |
|---|---|
| `style=""` (inline) | 1-0-0-0 |
| `#id` | 0-1-0-0 |
| `.class`, `[attr]`, `:pseudo-classe` | 0-0-1-0 |
| `element`, `::pseudo-élément` | 0-0-0-1 |
| `*` (universel) | 0-0-0-0 |

```css
p      { color: black; }   /* 0-0-0-1 */
.intro { color: blue; }    /* 0-0-1-0 — gagne */
#hero  { color: green; }   /* 0-1-0-0 — gagne */
p { color: red !important; } /* gagne sur tout — option nucléaire, à éviter */
```

> Si tu te retrouves à écrire `!important`, le vrai problème est la structure de tes sélecteurs. Corrige ça plutôt.

### L'héritage

```css
/* Hérité par défaut : color, font-*, line-height, visibility... */
body { font-family: sans-serif; color: #333; }

/* Forcer l'héritage */
.enfant { border: inherit; }

/* Réinitialiser */
.reset { font-size: initial; }
```

📖 https://www.w3schools.com/css/css_specificity.asp  
📖 https://www.w3schools.com/css/css_inherit.asp

---

## 4. Unités

| Unité | Relative à | Utiliser quand |
|---|---|---|
| `px` | Pixels | Bordures, ombres, tailles fixes |
| `rem` | `font-size` de `<html>` | Espacement cohérent sur toute la page |
| `em` | `font-size` de l'élément parent | Espacement relatif au texte local |
| `%` | Valeur du parent | Largeurs, hauteurs, positionnement |
| `vw` / `vh` | 1% de la largeur/hauteur du viewport | Sections pleine page |
| `ch` | Largeur du caractère `0` | Contrôle de la longueur de ligne |
| `dvh` | Hauteur viewport dynamique | Mobile (tient compte de la barre du navigateur) |

```css
html { font-size: 16px; }   /* base : 1rem = 16px */
h1   { font-size: 2rem; }   /* 32px — s'adapte si html change */

.container {
    max-width: 1200px;
    width: 90%;
}

.hero { height: 100vh; min-height: 600px; }
.texte { max-width: 65ch; line-height: 1.5; }
```

📖 https://www.w3schools.com/cssref/css_units.php

---

## 5. Couleurs

```css
color: red;                          /* couleur nommée */
color: #ff0000;                      /* hexadécimal */
color: #f00;                         /* raccourci */
color: rgb(255, 0, 0);               /* rgb */
color: rgb(255 0 0 / 50%);           /* rgb avec transparence */
color: hsl(0, 100%, 50%);            /* teinte, saturation, luminosité */
color: oklch(0.63 0.26 29);          /* espace colorimétrique moderne */
color: transparent;
color: currentColor;                  /* hérite de la couleur du texte courant */
```

📖 https://www.w3schools.com/css/css_colors.asp  
📖 https://www.w3schools.com/cssref/css_colors_legal.php

---

## 6. Le modèle de boîte

```css
/* Réinitialisation essentielle — mettre dans tout projet */
*, *::before, *::after { box-sizing: border-box; }
/*
  content-box (défaut) : width = contenu seulement, padding et border s'ajoutent
  border-box           : width INCLUT padding et border — logique pour l'humain
*/

.boite {
    width: 300px;
    padding: 16px;                   /* tous côtés */
    padding: 8px 16px;               /* haut/bas  gauche/droite */
    padding: 4px 8px 12px 16px;     /* haut droite bas gauche (dans le sens des aiguilles) */
    border: 2px solid #333;
    margin: 24px auto;               /* auto horizontalement = centre le bloc */
}
```

📖 https://www.w3schools.com/css/css_boxmodel.asp  
📖 https://www.w3schools.com/cssref/pr_box-sizing.php

---

## 7. Typographie et polices

```css
body {
    font-family: "Inter", "Segoe UI", Arial, sans-serif;
    font-size: 1rem;
    font-weight: 400;
    line-height: 1.5;     /* sans unité — préféré */
    color: #333;
}

h1 { font-size: clamp(1.5rem, 4vw, 3rem); }  /* taille fluide */

.titre    { text-transform: uppercase; }
.centre   { text-align: center; }
.tronquer {
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;  /* affiche "..." quand le texte dépasse */
}

/* Police web personnalisée */
@font-face {
    font-family: "MaPolice";
    src: url("fonts/mapolice.woff2") format("woff2");
    font-display: swap;
}
```

📖 https://www.w3schools.com/css/css_font.asp  
📖 https://www.w3schools.com/cssref/pr_text_text-overflow.php

---

## 8. Arrière-plans

```css
.element {
    background-color: #f5f5f5;
    background-image: url("photo.jpg");
    background-repeat: no-repeat;
    background-position: center;
    background-size: cover;        /* remplit — peut rogner */
    background-size: contain;      /* ajuste — peut laisser de l'espace */
    background-attachment: fixed;  /* effet parallaxe */

    /* Raccourci */
    background: #1a1a2e url("bg.jpg") no-repeat center / cover;
}

/* Dégradés */
.degrade {
    background: linear-gradient(135deg, #667eea, #764ba2);
    background: radial-gradient(circle, #f093fb, #f5576c);
    background: conic-gradient(from 90deg, red, yellow, green, blue, red);
}
```

📖 https://www.w3schools.com/css/css_background.asp  
📖 https://www.w3schools.com/css/css3_gradients.asp

---

## 9. Bordures et contours

```css
.element {
    border: 1px solid #ccc;          /* raccourci : épaisseur style couleur */
    border-radius: 8px;              /* coins arrondis */
    border-radius: 50%;              /* cercle parfait */
    box-shadow: 0 2px 4px rgba(0,0,0,0.1);  /* ombre */
    box-shadow: inset 0 2px 4px rgba(0,0,0,0.1); /* ombre intérieure */
}

/* Outline — en dehors de la boîte, ne modifie pas le flux */
:focus {
    outline: 2px solid #6c63ff;
    outline-offset: 3px;
}
/* Ne jamais faire : *:focus { outline: none; } — nuit à l'accessibilité */
```

📖 https://www.w3schools.com/css/css_border.asp  
📖 https://www.w3schools.com/cssref/pr_box-shadow.php

---

## 10. Display et visibilité

```css
display: block;       /* pleine largeur, nouvelle ligne */
display: inline;      /* dans le flux du texte */
display: inline-block;/* dans le flux + accepte width/height */
display: none;        /* supprimé du layout */
display: flex;
display: grid;

visibility: hidden;   /* invisible mais occupe l'espace */
opacity: 0;           /* invisible, occupe l'espace ET reste cliquable */

/* Masquer accessiblement (visible pour les lecteurs d'écran) */
.sr-only {
    position: absolute;
    width: 1px; height: 1px;
    overflow: hidden;
    clip: rect(0,0,0,0);
    white-space: nowrap;
}
```

📖 https://www.w3schools.com/cssref/pr_class_display.php

---

## 11. Flexbox

Flexbox est pour la mise en page **unidimensionnelle** — une ligne OU une colonne.

```css
/* Conteneur */
.flex-conteneur {
    display: flex;
    flex-direction: row;              /* row | column | row-reverse | column-reverse */
    flex-wrap: wrap;                  /* nowrap | wrap */
    justify-content: space-between;  /* axe principal */
    align-items: center;             /* axe transversal */
    gap: 16px;
}

/* Éléments */
.flex-element {
    flex: 1;             /* grandit pour remplir l'espace */
    flex: none;          /* rigide */
    flex: 0 0 200px;    /* 200px fixes */
    align-self: flex-end;
    order: -1;           /* réordonner visuellement */
}
```

📖 https://www.w3schools.com/css/css3_flexbox.asp

---

## 12. CSS Grid

Grid est pour la mise en page **bidimensionnelle** — lignes ET colonnes.

```css
/* Conteneur */
.grille {
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    grid-template-columns: repeat(auto-fill, minmax(250px, 1fr)); /* responsive */
    gap: 24px;

    /* Zones nommées */
    grid-template-areas:
        "entete  entete"
        "sidebar contenu"
        "pied    pied";
}

.entete  { grid-area: entete; }
.sidebar { grid-area: sidebar; }
.contenu { grid-area: contenu; }
.pied    { grid-area: pied; }

/* Éléments */
.element {
    grid-column: 1 / 3;     /* de la ligne 1 à la ligne 3 */
    grid-column: span 2;    /* s'étend sur 2 colonnes */
    grid-row: 1 / -1;       /* toute la hauteur */
}
```

📖 https://www.w3schools.com/css/css_grid.asp

---

## 13. Positionnement

```css
position: static;    /* défaut — dans le flux normal */
position: relative;  /* décalé par rapport à sa position normale */
position: absolute;  /* hors du flux, relatif à l'ancêtre non-static le plus proche */
position: fixed;     /* hors du flux, relatif au viewport — reste au scroll */
position: sticky;    /* hybride — relatif puis fixe quand le seuil est atteint */

/* Centrer avec absolute */
.centre {
    position: absolute;
    top: 50%; left: 50%;
    transform: translate(-50%, -50%);
}

/* Couvrir tout le parent */
.overlay {
    position: absolute;
    inset: 0;  /* raccourci pour top:0 right:0 bottom:0 left:0 */
}

/* z-index — ordre d'empilement */
/* Ne fonctionne que sur les éléments positionnés (non static) */
.nav     { z-index: 100; }
.modal   { z-index: 200; }
.tooltip { z-index: 300; }
```

📖 https://www.w3schools.com/css/css_positioning.asp  
📖 https://www.w3schools.com/cssref/pr_pos_z-index.php

---

## 14. Dimensionnement

```css
width: 300px;
width: 50%;
width: auto;
width: fit-content;
width: min-content;
width: max-content;

min-width: 200px;
max-width: 1200px;

/* Rapport d'aspect */
aspect-ratio: 16 / 9;
aspect-ratio: 1;        /* carré */

/* Pour <img> et <video> dans un conteneur dimensionné */
object-fit: cover;      /* remplit — peut rogner */
object-fit: contain;    /* ajuste sans rogner */
```

📖 https://www.w3schools.com/cssref/pr_object-fit.php

---

## 15. Overflow

```css
overflow: visible;   /* dépasse (défaut) */
overflow: hidden;    /* rogner */
overflow: scroll;    /* scrollbar toujours visible */
overflow: auto;      /* scrollbar seulement si nécessaire — préférer */
overflow-x: auto;
overflow-y: hidden;
scroll-behavior: smooth;
```

📖 https://www.w3schools.com/cssref/pr_pos_overflow.php

---

## 16. Transformations

Les transformations déplacent, tournent, redimensionnent ou déforment les éléments **sans affecter le flux**.

```css
transform: translateX(50px);
transform: translate(50px, -20px);
transform: scale(1.5);
transform: rotate(45deg);
transform: skew(15deg, 10deg);

/* Chaînage — appliqué de droite à gauche */
transform: translate(50px, 50px) rotate(45deg) scale(1.5);

/* Origine de la transformation */
transform-origin: top left;
transform-origin: 0 0;

/* 3D */
transform: rotateY(45deg);
perspective: 1000px;
```

📖 https://www.w3schools.com/css/css3_2dtransforms.asp  
📖 https://www.w3schools.com/cssref/pr_transform.php

---

## 17. Transitions

Les transitions animent les changements entre deux états.

```css
.bouton {
    background-color: #6c63ff;
    transition: background-color 0.3s ease;

    /* Plusieurs propriétés */
    transition:
        background-color 0.3s ease,
        transform 0.15s ease-out,
        box-shadow 0.3s ease;
}

.bouton:hover {
    background-color: #5a52d5;
    transform: translateY(-2px);
}
```

**Fonctions de temporisation :**

```css
transition-timing-function: linear;
transition-timing-function: ease;           /* défaut */
transition-timing-function: ease-in;
transition-timing-function: ease-out;
transition-timing-function: ease-in-out;
transition-timing-function: cubic-bezier(0.68, -0.55, 0.27, 1.55);
```

📖 https://www.w3schools.com/css/css3_transitions.asp

---

## 18. Animations et keyframes

Les animations s'exécutent automatiquement, peuvent boucler et offrent un contrôle total sur la timeline.

```css
/* Définir l'animation */
@keyframes apparition {
    from { opacity: 0; transform: translateY(20px); }
    to   { opacity: 1; transform: translateY(0); }
}

@keyframes rotation {
    from { transform: rotate(0deg); }
    to   { transform: rotate(360deg); }
}

/* Appliquer */
.carte {
    animation: apparition 0.5s ease-out 0.2s 1 normal forwards;
    /* nom durée timing délai répétitions direction fill-mode */
}

.chargement {
    animation: rotation 1s linear infinite;
}

/* Respecter la préférence utilisateur */
@media (prefers-reduced-motion: reduce) {
    * {
        animation-duration: 0.01ms !important;
        transition-duration: 0.01ms !important;
    }
}
```

📖 https://www.w3schools.com/css/css3_animations.asp  
📖 https://www.w3schools.com/cssref/atrule_keyframes.php

---

## 19. Pseudo-classes

```css
/* États d'interaction */
a:link    { color: blue; }
a:visited { color: purple; }
a:hover   { color: darkblue; }
a:active  { color: red; }
/* Ordre : L-V-H-A (LoVe HAte) */

:focus          { outline: 2px solid blue; }
:focus-visible  { outline: 2px solid blue; }   /* focus clavier uniquement */

input:disabled  { background: #ccc; }
input:checked   { accent-color: #6c63ff; }
input:valid     { border-color: green; }
input:invalid   { border-color: red; }

/* Structurelles */
li:first-child     { font-weight: bold; }
li:last-child      { border-bottom: none; }
li:nth-child(odd)  { background: #f9f9f9; }
li:nth-child(3n+1) { color: blue; }
p:not(.intro)      { color: #555; }

/* :has — sélecteur parent (CSS4) */
.card:has(img)          { padding: 0; }
form:has(:invalid)      { border-color: red; }
```

📖 https://www.w3schools.com/css/css_pseudo_classes.asp  
📖 https://www.w3schools.com/cssref/sel_has.php

---

## 20. Pseudo-éléments

```css
/* ::before et ::after — insérer du contenu */
.bouton::before {
    content: "→ ";
}
.requis::after {
    content: " *";
    color: red;
}

/* Décoration pure */
.diviseur::before {
    content: "";
    display: block;
    height: 2px;
    background: linear-gradient(to right, transparent, #6c63ff, transparent);
}

::first-letter { font-size: 3em; float: left; }  /* lettrine */
::placeholder  { color: #999; font-style: italic; }
::selection    { background: #6c63ff; color: white; }
li::marker     { color: #6c63ff; }
dialog::backdrop { background: rgba(0,0,0,0.6); }
```

📖 https://www.w3schools.com/css/css_pseudo_elements.asp

---

## 21. Propriétés personnalisées — Variables CSS

```css
:root {
    --couleur-principale: #6c63ff;
    --couleur-texte:      #1a1a2e;
    --police-sans: "Inter", system-ui, sans-serif;
    --espacement-4: 1rem;
    --rayon-md: 8px;
    --ombre-md: 0 4px 6px -1px rgba(0,0,0,0.1);
    --duree-normale: 300ms;
}

.bouton {
    background-color: var(--couleur-principale);
    padding: var(--espacement-4);
    border-radius: var(--rayon-md);
    transition: background-color var(--duree-normale) ease;
}

/* Valeur de repli */
color: var(--couleur-accent, #6c63ff);

/* Surcharge locale */
.carte { --couleur-principale: #f5576c; }

/* Mode sombre */
@media (prefers-color-scheme: dark) {
    :root {
        --couleur-texte: #f9fafb;
        --couleur-bg:    #1a1a2e;
    }
}

/* Utilisation dans calc() */
padding: calc(var(--espacement-4) * 2);
```

📖 https://www.w3schools.com/css/css3_variables.asp

---

## 22. Fonctions CSS

```css
/* calc() — arithmétique avec unités mixtes */
width: calc(100% - 48px);
height: calc(100vh - 80px);

/* min() — la plus petite valeur */
width: min(100%, 1200px);

/* max() — la plus grande valeur */
padding: max(16px, 4vw);

/* clamp(min, préféré, max) — plage responsive */
font-size: clamp(1rem, 2.5vw, 2rem);
width: clamp(300px, 80%, 1200px);

/* var() — variables CSS */
color: var(--couleur-principale, #6c63ff);

/* repeat() et minmax() — Grid */
grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
grid-auto-rows: minmax(100px, auto);

/* Nouvelles fonctions de transformation (CSS Level 5) */
translate: 10px 20px;
rotate: 45deg;
scale: 1.5;
```

📖 https://www.w3schools.com/cssref/css_functions.php  
📖 https://www.w3schools.com/cssref/func_minmax.php

---

## 23. Règles @

```css
@import url("variables.css");

@font-face {
    font-family: "MaPolice";
    src: url("police.woff2") format("woff2");
    font-display: swap;
}

@keyframes nom { ... }

@media (min-width: 768px) { ... }

@layer reset, base, composants, utilitaires;
@layer reset { * { margin: 0; padding: 0; } }

@supports (display: grid) { .conteneur { display: grid; } }

@container (min-width: 400px) { .carte { flex-direction: row; } }

@media print {
    .no-print { display: none; }
    body { font-size: 12pt; color: black; }
}
```

📖 https://www.w3schools.com/cssref/atrule_font-face.php  
📖 https://www.w3schools.com/cssref/atrule_supports.php

---

## 24. Media Queries et design responsive

```css
/* Approche mobile-first — styles de base = petit écran */
.conteneur { padding: 16px; }

@media (min-width: 640px) { .conteneur { padding: 24px; } }
@media (min-width: 768px) {
    .conteneur { padding: 32px; }
    .grille { grid-template-columns: 1fr 1fr; }
}
@media (min-width: 1024px) {
    .conteneur { max-width: 1024px; margin: auto; }
}

/* Syntaxe moderne — plus lisible */
@media (width >= 768px)  { ... }
@media (768px <= width <= 1024px) { ... }

/* Autres conditions */
@media (prefers-color-scheme: dark)    { ... }  /* mode sombre */
@media (prefers-reduced-motion: reduce){ ... }  /* moins d'animation */
@media (hover: none)                   { ... }  /* appareil tactile */
@media print                           { ... }  /* impression */

/* Typographie fluide */
html { font-size: clamp(14px, 1.5vw, 18px); }

/* Conteneur fluide */
.conteneur {
    width: min(100% - 32px, 1200px);
    margin-inline: auto;
}
```

📖 https://www.w3schools.com/css/css_rwd_mediaqueries.asp

---

## 25. Carte de référence rapide

### Sélecteurs

| Sélecteur | Cible |
|---|---|
| `*` | Tous les éléments |
| `div` | Tous les `<div>` |
| `.classe` | Éléments avec la classe |
| `#id` | Élément avec l'id |
| `div p` | `<p>` dans `<div>` (toute profondeur) |
| `div > p` | Enfants `<p>` directs de `<div>` |
| `h2 + p` | `<p>` immédiatement après `<h2>` |
| `:hover` | Survol souris |
| `:focus` | Focus clavier |
| `:nth-child(n)` | nième enfant |
| `:not(x)` | Ne correspond pas à x |
| `:has(x)` | Contient x |
| `::before` | Élément virtuel avant le contenu |
| `::after` | Élément virtuel après le contenu |

### Ce qu'on confond souvent

| | |
|---|---|
| `margin` vs `padding` | margin = extérieur (transparent), padding = intérieur |
| `width: 100%` | relatif au parent — le parent doit avoir une largeur définie |
| `height: 100%` | ne fonctionne que si le parent a une hauteur explicite |
| `position: absolute` | relatif à l'ancêtre non-static le plus proche |
| `z-index` | ne fonctionne que sur les éléments positionnés (non static) |
| `display: none` | supprime du layout et de l'accessibilité |
| `visibility: hidden` | invisible mais occupe l'espace |
| `opacity: 0` | invisible, occupe l'espace ET reste cliquable |
| `em` | relatif au `font-size` de l'élément courant |
| `rem` | relatif au `font-size` de `<html>` |
| `transition` | anime entre DEUX états (avant/après) |
| `animation` | anime sur une TIMELINE (keyframes), peut boucler |
| `:hover` | pseudo-classe d'interaction |
| `::before` | pseudo-élément de contenu généré |
