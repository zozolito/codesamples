# CSS — Cascading Style Sheets

---

## 🇬🇧 English

### What is CSS?

CSS (Cascading Style Sheets) is the language that controls how HTML elements look on screen. It handles colors, fonts, spacing, layout, animations, and responsive behavior. It is not a programming language in the traditional sense — it has no variables in its original form, no loops, no functions — and yet it is one of the most powerful and misunderstood tools in web development.

The "Cascading" part is what trips most people up. Styles inherit, override, and compete with each other based on specificity and order. Understanding the cascade is the difference between CSS that works and CSS that is a 400-line nightmare of `!important`.

---

### A brief history

| Year | Event |
|------|-------|
| 1994 | Håkon Wium Lie proposes CSS while working at CERN |
| 1996 | CSS1 becomes a W3C recommendation |
| 1998 | CSS2 released — adds positioning, media types |
| 2011 | CSS3 arrives in modules — transitions, animations, flexbox |
| 2017 | CSS Grid becomes widely supported — layout finally makes sense |
| 2022+ | Container queries, cascade layers, `:has()` — CSS keeps evolving |

> For years, developers used JavaScript and `<table>` elements to do layouts. CSS Grid and Flexbox ended that era. If you ever see a layout built with `<table>`, run.

---

### Best tools

| Tool | Purpose | Link |
|------|---------|-------|
| **VS Code** | Great CSS IntelliSense and preview | https://code.visualstudio.com |
| **Browser DevTools** | Inspect and live-edit CSS in the browser | F12 |
| **Firefox DevTools** | Best CSS Grid and Flexbox inspector | F12 in Firefox |
| **PostCSS** | Transform CSS with plugins (autoprefixer, etc.) | https://postcss.org |
| **Sass** | CSS with superpowers — variables, nesting, mixins | https://sass-lang.com |
| **Tailwind CSS** | Utility-first CSS framework | https://tailwindcss.com |
| **Coolors** | Color palette generator | https://coolors.co |
| **Can I Use** | Browser compatibility checker | https://caniuse.com |

---

### Basic syntax

#### The basics — selectors and properties

```css
/* This is a comment in CSS */

/* Element selector */
p {
    color: #333333;
    font-size: 16px;
    line-height: 1.5;
}

/* Class selector — reusable */
.card {
    background-color: white;
    border-radius: 8px;
    padding: 16px;
    box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

/* ID selector — unique per page, avoid overusing */
#header {
    background-color: #1a1a2e;
    color: white;
}

/* Multiple selectors */
h1, h2, h3 {
    font-family: "Georgia", serif;
    font-weight: bold;
}
```

#### The Box Model — the most important concept in CSS

```css
/*
  Every element is a box with:
  - content  : the actual content
  - padding  : space inside the border
  - border   : the border itself
  - margin   : space outside the border
*/

.box {
    width: 200px;
    height: 100px;
    padding: 16px;        /* all sides */
    border: 2px solid black;
    margin: 24px auto;    /* top/bottom: 24px, left/right: auto (centers the box) */

    /* box-sizing: border-box makes width/height include padding and border */
    /* Always use this — it prevents headaches */
    box-sizing: border-box;
}
```

#### Colors

```css
.colors {
    color: red;                        /* named color */
    color: #ff0000;                    /* hex */
    color: rgb(255, 0, 0);             /* rgb */
    color: rgba(255, 0, 0, 0.5);       /* rgb with transparency */
    color: hsl(0, 100%, 50%);          /* hue, saturation, lightness */
    color: oklch(0.63 0.26 29.23);     /* modern color space — more perceptually uniform */
}
```

#### Flexbox — one-dimensional layout

```css
/* The container */
.flex-container {
    display: flex;
    flex-direction: row;          /* row | column */
    justify-content: space-between; /* align along main axis */
    align-items: center;          /* align along cross axis */
    gap: 16px;                    /* space between children */
    flex-wrap: wrap;              /* allow wrapping to next line */
}

/* The children */
.flex-item {
    flex: 1;     /* grow to fill available space equally */
}

.flex-item-fixed {
    flex: 0 0 200px; /* don't grow, don't shrink, stay at 200px */
}
```

#### CSS Grid — two-dimensional layout

```css
/* The container */
.grid-container {
    display: grid;
    grid-template-columns: repeat(3, 1fr); /* 3 equal columns */
    grid-template-rows: auto;
    gap: 24px;
}

/* A child spanning multiple columns */
.featured {
    grid-column: span 2; /* takes 2 columns */
}

/* Named areas — very readable */
.page-layout {
    display: grid;
    grid-template-areas:
        "header header"
        "sidebar main"
        "footer footer";
    grid-template-columns: 250px 1fr;
}

.header  { grid-area: header; }
.sidebar { grid-area: sidebar; }
.main    { grid-area: main; }
.footer  { grid-area: footer; }
```

#### Responsive design — media queries

```css
/* Mobile first: base styles are for small screens */
.container {
    padding: 16px;
    font-size: 14px;
}

/* Then add styles for larger screens */
@media (min-width: 768px) {
    .container {
        padding: 32px;
        font-size: 16px;
    }
}

@media (min-width: 1200px) {
    .container {
        max-width: 1140px;
        margin: 0 auto;
    }
}
```

#### Custom properties (CSS variables)

```css
/* Define at the root — available everywhere */
:root {
    --color-primary: #6c63ff;
    --color-text: #333333;
    --spacing-md: 16px;
    --border-radius: 8px;
}

.button {
    background-color: var(--color-primary);
    padding: var(--spacing-md);
    border-radius: var(--border-radius);
    color: white;
}
```

#### Transitions and animations

```css
/* Transition — smooth change between two states */
.button {
    background-color: #6c63ff;
    transition: background-color 0.2s ease, transform 0.1s ease;
}

.button:hover {
    background-color: #5a52d5;
    transform: translateY(-2px); /* subtle lift on hover */
}

/* Keyframe animation */
@keyframes fadeIn {
    from {
        opacity: 0;
        transform: translateY(10px);
    }
    to {
        opacity: 1;
        transform: translateY(0);
    }
}

.card {
    animation: fadeIn 0.3s ease forwards;
}
```

#### A common mistake: specificity wars

```css
/* Bad — using !important is almost always a sign something is wrong */
.button {
    color: red !important; /* don't do this */
}

/* The real fix: understand specificity
   inline styles > #id > .class > element
   Keep specificity low and consistent
*/
.button {
    color: red; /* works fine if no higher specificity rule overrides it */
}
```

---

## 🇫🇷 Français

### C'est quoi le CSS ?

Le CSS (Cascading Style Sheets — feuilles de style en cascade) est le langage qui contrôle l'apparence des éléments HTML à l'écran. Il gère les couleurs, les polices, l'espacement, la mise en page, les animations et le comportement responsive. Ce n'est pas un langage de programmation au sens traditionnel — dans sa forme originale il n'a pas de variables, pas de boucles, pas de fonctions — et pourtant c'est l'un des outils les plus puissants et les plus mal compris du développement web.

La partie "Cascading" est ce qui fait trébucher la plupart des gens. Les styles héritent, se substituent les uns aux autres et entrent en concurrence selon la spécificité et l'ordre. Comprendre la cascade, c'est la différence entre un CSS qui fonctionne et un CSS qui est un cauchemar de 400 lignes de `!important`.

---

### Un peu d'histoire

| Année | Événement |
|-------|-----------|
| 1994 | Håkon Wium Lie propose le CSS en travaillant au CERN |
| 1996 | CSS1 devient une recommandation W3C |
| 1998 | CSS2 sort — ajoute le positionnement, les types de médias |
| 2011 | CSS3 arrive en modules — transitions, animations, flexbox |
| 2017 | CSS Grid est largement supporté — le layout devient enfin logique |
| 2022+ | Container queries, cascade layers, `:has()` — le CSS continue d'évoluer |

> Pendant des années, les développeurs utilisaient JavaScript et des éléments `<table>` pour faire des mises en page. CSS Grid et Flexbox ont mis fin à cette époque. Si tu vois un layout construit avec des `<table>`, cours.

---

### Les meilleurs outils

| Outil | Utilité | Lien |
|-------|---------|------|
| **VS Code** | Excellent IntelliSense et aperçu CSS | https://code.visualstudio.com |
| **DevTools navigateur** | Inspecter et modifier le CSS en direct | F12 |
| **Firefox DevTools** | Meilleur inspecteur Grid et Flexbox | F12 dans Firefox |
| **PostCSS** | Transformer le CSS avec des plugins | https://postcss.org |
| **Sass** | CSS avec des super-pouvoirs — variables, imbrication, mixins | https://sass-lang.com |
| **Tailwind CSS** | Framework CSS utilitaire | https://tailwindcss.com |
| **Coolors** | Générateur de palettes de couleurs | https://coolors.co |
| **Can I Use** | Vérificateur de compatibilité navigateur | https://caniuse.com |

---

### Syntaxe de base

#### Les bases — sélecteurs et propriétés

```css
/* Ceci est un commentaire en CSS */

/* Sélecteur d'élément */
p {
    color: #333333;
    font-size: 16px;
    line-height: 1.5;
}

/* Sélecteur de classe — réutilisable */
.carte {
    background-color: white;
    border-radius: 8px;
    padding: 16px;
    box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

/* Sélecteur d'ID — unique par page, évite de trop l'utiliser */
#entete {
    background-color: #1a1a2e;
    color: white;
}

/* Sélecteurs multiples */
h1, h2, h3 {
    font-family: "Georgia", serif;
    font-weight: bold;
}
```

#### Le modèle de boîte — le concept le plus important en CSS

```css
/*
  Chaque élément est une boîte avec :
  - content  : le contenu réel
  - padding  : espace à l'intérieur de la bordure
  - border   : la bordure elle-même
  - margin   : espace à l'extérieur de la bordure
*/

.boite {
    width: 200px;
    height: 100px;
    padding: 16px;
    border: 2px solid black;
    margin: 24px auto; /* haut/bas: 24px, gauche/droite: auto (centre la boîte) */

    /* box-sizing: border-box fait que width/height incluent padding et border */
    /* Utilise toujours ça — ça évite les maux de tête */
    box-sizing: border-box;
}
```

#### Couleurs

```css
.couleurs {
    color: red;                        /* couleur nommée */
    color: #ff0000;                    /* hexadécimal */
    color: rgb(255, 0, 0);             /* rgb */
    color: rgba(255, 0, 0, 0.5);       /* rgb avec transparence */
    color: hsl(0, 100%, 50%);          /* teinte, saturation, luminosité */
    color: oklch(0.63 0.26 29.23);     /* espace colorimétrique moderne */
}
```

#### Flexbox — mise en page unidimensionnelle

```css
/* Le conteneur */
.conteneur-flex {
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    align-items: center;
    gap: 16px;
    flex-wrap: wrap;
}

/* Les enfants */
.element-flex {
    flex: 1; /* grandit pour remplir l'espace disponible également */
}
```

#### CSS Grid — mise en page bidimensionnelle

```css
.grille {
    display: grid;
    grid-template-columns: repeat(3, 1fr); /* 3 colonnes égales */
    gap: 24px;
}

/* Zones nommées — très lisible */
.mise-en-page {
    display: grid;
    grid-template-areas:
        "entete entete"
        "sidebar contenu"
        "pied    pied";
    grid-template-columns: 250px 1fr;
}
```

#### Design responsive — media queries

```css
/* Mobile d'abord : les styles de base sont pour les petits écrans */
.conteneur {
    padding: 16px;
    font-size: 14px;
}

/* Puis on ajoute les styles pour les écrans plus grands */
@media (min-width: 768px) {
    .conteneur {
        padding: 32px;
        font-size: 16px;
    }
}
```

#### Propriétés personnalisées (variables CSS)

```css
/* Définies à la racine — disponibles partout */
:root {
    --couleur-principale: #6c63ff;
    --couleur-texte: #333333;
    --espacement-md: 16px;
    --rayon-bordure: 8px;
}

.bouton {
    background-color: var(--couleur-principale);
    padding: var(--espacement-md);
    border-radius: var(--rayon-bordure);
    color: white;
}
```

#### Transitions et animations

```css
/* Transition — changement fluide entre deux états */
.bouton {
    background-color: #6c63ff;
    transition: background-color 0.2s ease, transform 0.1s ease;
}

.bouton:hover {
    background-color: #5a52d5;
    transform: translateY(-2px);
}

/* Animation par images clés */
@keyframes apparition {
    from { opacity: 0; transform: translateY(10px); }
    to   { opacity: 1; transform: translateY(0); }
}

.carte {
    animation: apparition 0.3s ease forwards;
}
```
