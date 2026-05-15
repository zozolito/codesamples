# JavaScript

---

## 🇬🇧 English

### What is JavaScript?

JavaScript is the language of the web. It runs in every browser, on every platform, without installation. It started as a scripting tool to make web pages interactive and became one of the most widely used programming languages in the world — for better and for worse.

Today JavaScript runs in the browser (frontend), on the server (Node.js), in mobile apps (React Native), in desktop apps (Electron), and even in IoT devices. It is everywhere. Which means the consequences of writing it badly are also everywhere.

---

### A brief history

| Year | Event |
|------|-------|
| 1995 | Brendan Eich creates JavaScript at Netscape — in **10 days**. Yes, 10 days. |
| 1997 | ECMAScript standard is born (the official spec behind JS) |
| 2009 | Node.js released — JS escapes the browser |
| 2015 | ES6 (ES2015) — the modern era begins: `let`, `const`, arrow functions, classes, modules |
| 2016–now | Yearly ECMAScript releases, async/await, optional chaining, and much more |

> Fun fact: JavaScript has no relation to Java. The name was pure marketing. Java was popular in 1995 and Netscape wanted some of that hype. Don't let anyone confuse you.

---

### Best tools

| Tool | Purpose | Link |
|------|---------|-------|
| **VS Code** | The de facto standard editor for JS | https://code.visualstudio.com |
| **Node.js** | Run JS outside the browser | https://nodejs.org |
| **ESLint** | Catch errors and enforce style | https://eslint.org |
| **Prettier** | Auto-format your code | https://prettier.io |
| **Vite** | Fast dev server and bundler | https://vitejs.dev |
| **Chrome DevTools** | Built into Chrome — inspect, debug, profile | F12 in your browser |
| **Firefox DevTools** | Excellent CSS and JS debugger | F12 in Firefox |

---

### Basic syntax

#### Variables

```javascript
// Use const by default
const name = "Alice";

// Use let when the value will change
let score = 0;
score = 10;

// Never use var — it has confusing scoping rules
// var legacy = "don't do this";
```

#### Data types

```javascript
const text = "Hello";           // string
const number = 42;              // number (integers and floats are the same type)
const decimal = 3.14;           // number
const flag = true;              // boolean
const nothing = null;           // null (intentional absence of value)
let notDefined;                 // undefined (value not assigned)
const obj = { key: "value" };   // object
const arr = [1, 2, 3];          // array (also an object)
```

#### Functions

```javascript
// Classic function declaration
function greet(name) {
    return "Hello, " + name;
}

// Arrow function (modern, preferred for short functions)
const greet = (name) => `Hello, ${name}`;

// Default parameters
const greet = (name = "stranger") => `Hello, ${name}`;

console.log(greet("Alice")); // Hello, Alice
console.log(greet());        // Hello, stranger
```

#### Conditionals

```javascript
const age = 20;

if (age >= 18) {
    console.log("Adult");
} else if (age >= 13) {
    console.log("Teenager");
} else {
    console.log("Child");
}

// Ternary operator (for simple cases)
const label = age >= 18 ? "Adult" : "Minor";
```

#### Loops

```javascript
// Classic for loop
for (let i = 0; i < 5; i++) {
    console.log(i);
}

// Loop over an array — prefer this
const fruits = ["apple", "banana", "cherry"];

for (const fruit of fruits) {
    console.log(fruit);
}

// forEach (functional style)
fruits.forEach(fruit => console.log(fruit));
```

#### Arrays

```javascript
const numbers = [1, 2, 3, 4, 5];

// map — transform each element, returns new array
const doubled = numbers.map(n => n * 2); // [2, 4, 6, 8, 10]

// filter — keep elements that pass the test
const evens = numbers.filter(n => n % 2 === 0); // [2, 4]

// find — first element that passes the test
const firstBig = numbers.find(n => n > 3); // 4

// reduce — boil the array down to a single value
const sum = numbers.reduce((acc, n) => acc + n, 0); // 15
```

#### Objects

```javascript
const user = {
    name: "Alice",
    age: 30,
    greet() {
        return `Hi, I'm ${this.name}`;
    }
};

console.log(user.name);      // Alice
console.log(user["age"]);    // 30
console.log(user.greet());   // Hi, I'm Alice

// Destructuring
const { name, age } = user;
console.log(name); // Alice
```

#### Async / Await

```javascript
// Fetching data from an API — the modern way
async function getUser(id) {
    try {
        const response = await fetch(`https://api.example.com/users/${id}`);
        
        if (!response.ok) {
            throw new Error(`HTTP error: ${response.status}`);
        }
        
        const user = await response.json();
        return user;
    } catch (error) {
        console.error("Failed to fetch user:", error);
    }
}
```

#### A common security pitfall: never do this

```javascript
// DANGEROUS — never insert user input directly into HTML
const userInput = "<script>alert('hacked')</script>";
document.innerHTML = userInput; // XSS attack vector

// SAFE — use textContent for text, or sanitize HTML properly
document.textContent = userInput; // renders as plain text, not HTML
```

---

## 🇫🇷 Français

### C'est quoi JavaScript ?

JavaScript est le langage du web. Il tourne dans chaque navigateur, sur chaque plateforme, sans installation. Il a commencé comme un outil de script pour rendre les pages web interactives, et il est devenu l'un des langages de programmation les plus utilisés au monde — pour le meilleur et pour le pire.

Aujourd'hui JavaScript tourne dans le navigateur (frontend), sur le serveur (Node.js), dans les apps mobiles (React Native), dans les apps desktop (Electron), et même dans des appareils IoT. Il est partout. Ce qui signifie que les conséquences de le mal écrire sont aussi partout.

---

### Un peu d'histoire

| Année | Événement |
|-------|-----------|
| 1995 | Brendan Eich crée JavaScript chez Netscape — en **10 jours**. Oui, 10 jours. |
| 1997 | Le standard ECMAScript naît (la spec officielle derrière JS) |
| 2009 | Node.js sort — JS s'échappe du navigateur |
| 2015 | ES6 (ES2015) — l'ère moderne commence : `let`, `const`, fonctions fléchées, classes, modules |
| 2016–aujourd'hui | Sorties annuelles d'ECMAScript, async/await, optional chaining, et bien plus |

> Anecdote : JavaScript n'a aucun lien avec Java. Le nom était du pur marketing. Java était populaire en 1995 et Netscape voulait profiter de cet engouement. Ne laisse personne te confondre.

---

### Les meilleurs outils

| Outil | Utilité | Lien |
|-------|---------|------|
| **VS Code** | L'éditeur standard de facto pour JS | https://code.visualstudio.com |
| **Node.js** | Exécuter JS en dehors du navigateur | https://nodejs.org |
| **ESLint** | Détecter les erreurs et imposer un style | https://eslint.org |
| **Prettier** | Formater automatiquement le code | https://prettier.io |
| **Vite** | Serveur de dev rapide et bundler | https://vitejs.dev |
| **Chrome DevTools** | Intégré à Chrome — inspecter, déboguer, profiler | F12 dans le navigateur |
| **Firefox DevTools** | Excellent débogueur CSS et JS | F12 dans Firefox |

---

### Syntaxe de base

#### Variables

```javascript
// Utilise const par défaut
const name = "Alice";

// Utilise let quand la valeur va changer
let score = 0;
score = 10;

// N'utilise jamais var — ses règles de portée sont déroutantes
// var legacy = "ne fais pas ça";
```

#### Types de données

```javascript
const texte = "Bonjour";        // string
const nombre = 42;              // number (entiers et flottants sont le même type)
const decimal = 3.14;           // number
const flag = true;              // boolean
const rien = null;              // null (absence intentionnelle de valeur)
let nonDefini;                  // undefined (valeur non assignée)
const obj = { cle: "valeur" };  // object
const arr = [1, 2, 3];          // array (aussi un objet)
```

#### Fonctions

```javascript
// Déclaration de fonction classique
function saluer(nom) {
    return "Bonjour, " + nom;
}

// Fonction fléchée (moderne, préférée pour les fonctions courtes)
const saluer = (nom) => `Bonjour, ${nom}`;

// Paramètres par défaut
const saluer = (nom = "inconnu") => `Bonjour, ${nom}`;

console.log(saluer("Alice")); // Bonjour, Alice
console.log(saluer());        // Bonjour, inconnu
```

#### Conditions

```javascript
const age = 20;

if (age >= 18) {
    console.log("Adulte");
} else if (age >= 13) {
    console.log("Adolescent");
} else {
    console.log("Enfant");
}

// Opérateur ternaire (pour les cas simples)
const label = age >= 18 ? "Adulte" : "Mineur";
```

#### Boucles

```javascript
// Boucle for classique
for (let i = 0; i < 5; i++) {
    console.log(i);
}

// Parcourir un tableau — préfère cette forme
const fruits = ["pomme", "banane", "cerise"];

for (const fruit of fruits) {
    console.log(fruit);
}

// forEach (style fonctionnel)
fruits.forEach(fruit => console.log(fruit));
```

#### Tableaux

```javascript
const nombres = [1, 2, 3, 4, 5];

// map — transforme chaque élément, retourne un nouveau tableau
const doubles = nombres.map(n => n * 2); // [2, 4, 6, 8, 10]

// filter — garde les éléments qui passent le test
const pairs = nombres.filter(n => n % 2 === 0); // [2, 4]

// find — premier élément qui passe le test
const premierGrand = nombres.find(n => n > 3); // 4

// reduce — réduit le tableau à une seule valeur
const somme = nombres.reduce((acc, n) => acc + n, 0); // 15
```

#### Objets

```javascript
const utilisateur = {
    nom: "Alice",
    age: 30,
    saluer() {
        return `Bonjour, je suis ${this.nom}`;
    }
};

console.log(utilisateur.nom);      // Alice
console.log(utilisateur["age"]);   // 30
console.log(utilisateur.saluer()); // Bonjour, je suis Alice

// Déstructuration
const { nom, age } = utilisateur;
console.log(nom); // Alice
```

#### Async / Await

```javascript
// Récupérer des données depuis une API — la façon moderne
async function getUtilisateur(id) {
    try {
        const response = await fetch(`https://api.exemple.com/utilisateurs/${id}`);
        
        if (!response.ok) {
            throw new Error(`Erreur HTTP : ${response.status}`);
        }
        
        const utilisateur = await response.json();
        return utilisateur;
    } catch (erreur) {
        console.error("Échec de la récupération :", erreur);
    }
}
```

#### Un piège de sécurité courant : ne fais jamais ça

```javascript
// DANGEREUX — n'insère jamais une entrée utilisateur directement dans le HTML
const inputUtilisateur = "<script>alert('piraté')</script>";
document.innerHTML = inputUtilisateur; // vecteur d'attaque XSS

// SAFE — utilise textContent pour le texte, ou sanitise correctement le HTML
document.textContent = inputUtilisateur; // affiché comme texte brut, pas comme HTML
```
