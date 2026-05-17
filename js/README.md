# JavaScript — CodeSamples

---

## 🇬🇧 English

### What's in this folder

| File / Folder | What it is |
|---|---|
| `Reference.md` | Comprehensive annotated reference — 22 sections from variables to security |
| `examples/index.html` | Interactive demo page — every concept runs live in the browser |
| `examples/main.js` | The JavaScript powering the demo — heavily commented, structured by concept |
| `examples/style.css` | Layout and styling for the demo page |

### How to use it

1. Open `examples/index.html` in any modern browser — no build step, no server needed
2. Open DevTools (`F12` → Console) — additional output appears there
3. Click "Run demo" in each section to execute the code
4. Read `main.js` — the code you see in the `<pre>` blocks is the actual code running the demo
5. Read `Reference.md` — deeper explanations, edge cases, and security notes

### Topics covered

- Variables & Scoping (`const`, `let`, `var`, hoisting, TDZ, block scope)
- Data Types & Type Coercion (`===`, `typeof`, falsy values, explicit conversion)
- Functions (declarations, expressions, arrow functions, default params, rest, IIFE)
- Objects (literals, shorthand, computed keys, `Object.keys/values/entries`, freeze)
- Arrays (`map`, `filter`, `reduce`, `find`, `sort` pitfall, `flat`, `Set`)
- Destructuring (arrays, objects, renaming, defaults, nested, function params)
- Spread & Rest (merging, copying, function arguments, omit-property pattern)
- Template Literals (interpolation, multi-line, tagged templates, `String.raw`)
- Classes & Prototypes (private fields, getters, `static`, inheritance, `this`)
- Closures (private state, factory functions, the `var`-in-loop pitfall)
- Promises (`then/catch/finally`, `Promise.all/allSettled/race/any`)
- Async / Await (`try/catch`, sequential vs parallel, error propagation)
- DOM Manipulation (`createElement`, `textContent`, `classList`, `DocumentFragment`)
- Events (delegation, `event.key`, custom events, `removeEventListener`)
- Error Handling (`try/catch/finally`, custom error classes, `instanceof`)
- Optional Chaining & Nullish Coalescing (`?.`, `??`, `??=`, `||` vs `??`)
- Storage (`localStorage`, `sessionStorage`, JSON serialisation, QuotaExceededError)
- Security Pitfalls (XSS, `eval`, prototype pollution, storage token risk)
- Performance (debounce, throttle, `IntersectionObserver`, `DocumentFragment`)

---

## 🇫🇷 Français

### Ce qu'il y a dans ce dossier

| Fichier / Dossier | Description |
|---|---|
| `Reference.md` | Référence complète et annotée — 22 sections de la déclaration de variables à la sécurité |
| `examples/index.html` | Page de démonstration interactive — chaque concept s'exécute en direct dans le navigateur |
| `examples/main.js` | Le JavaScript qui fait tourner la démo — très commenté, structuré par concept |
| `examples/style.css` | Mise en page et style de la page de démonstration |

### Comment l'utiliser

1. Ouvre `examples/index.html` dans n'importe quel navigateur moderne — pas besoin de build ni de serveur
2. Ouvre les DevTools (`F12` → Console) — des sorties supplémentaires y apparaissent
3. Clique sur "Run demo" dans chaque section pour exécuter le code
4. Lis `main.js` — le code affiché dans les blocs `<pre>` est le vrai code qui fait tourner la démo
5. Lis `Reference.md` — explications plus profondes, cas limites et notes de sécurité

### Points clés à retenir

- **`const` par défaut, `let` pour les valeurs qui changent, jamais `var`**
- **Toujours `===`** — `==` fait de la coercition de type et produit des résultats surprenants
- **Les fonctions fléchées n'ont pas de `this` propre** — elles héritent du contexte englobant
- **`fetch()` ne rejette pas sur les erreurs HTTP** — toujours vérifier `response.ok`
- **Ne jamais utiliser `innerHTML` avec des données utilisateur** — risque XSS
- **Ne jamais utiliser `eval()`** — exécution de code arbitraire
- **`??` plutôt que `||`** pour les valeurs optionnelles — `||` remplace aussi `0`, `""` et `false`
- **`Promise.all` pour les requêtes parallèles** — `await` successifs sont séquentiels (plus lents)
