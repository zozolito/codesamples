# JavaScript Reference

> *"JavaScript is the only language where it's possible to be both a complete beginner and completely lost as an expert."*
> This reference exists so you understand what you write — not just that it works.

---

## 🇬🇧 English

### Table of Contents

1. [Variables & Scoping](#1-variables--scoping)
2. [Data Types & Type Coercion](#2-data-types--type-coercion)
3. [Functions](#3-functions)
4. [Objects](#4-objects)
5. [Arrays](#5-arrays)
6. [Destructuring](#6-destructuring)
7. [Spread & Rest](#7-spread--rest)
8. [Template Literals](#8-template-literals)
9. [Classes & Prototypes](#9-classes--prototypes)
10. [Closures & Scope](#10-closures--scope)
11. [Higher-Order Functions](#11-higher-order-functions)
12. [Promises](#12-promises)
13. [Async / Await](#13-async--await)
14. [Fetch API](#14-fetch-api)
15. [DOM Manipulation](#15-dom-manipulation)
16. [Events](#16-events)
17. [Error Handling](#17-error-handling)
18. [Optional Chaining & Nullish Coalescing](#18-optional-chaining--nullish-coalescing)
19. [Modules (ES Modules)](#19-modules-es-modules)
20. [Storage (localStorage / sessionStorage)](#20-storage-localstorage--sessionstorage)
21. [Security Pitfalls](#21-security-pitfalls)
22. [Performance Patterns](#22-performance-patterns)

---

### 1. Variables & Scoping

JavaScript has three ways to declare variables. They differ in **scope**, **hoisting**, and **mutability**.

```javascript
// ─── var ────────────────────────────────────────────────────────────────────
// Function-scoped (NOT block-scoped). Hoisted to top of function with value
// `undefined`. Avoid in modern code — it creates confusing bugs.

function oldStyle() {
    console.log(x); // undefined — hoisted, not ReferenceError!
    var x = 10;
    if (true) {
        var x = 20; // same variable — overwrites the outer x!
    }
    console.log(x); // 20 — surprising!
}

// ─── let ────────────────────────────────────────────────────────────────────
// Block-scoped. Hoisted but NOT initialised → Temporal Dead Zone (TDZ).
// Use for values that change.

function modernStyle() {
    // console.log(y); // ReferenceError: Cannot access 'y' before initialisation
    let y = 10;
    if (true) {
        let y = 20; // different variable — scoped to this block
    }
    console.log(y); // 10 — correct!
}

// ─── const ──────────────────────────────────────────────────────────────────
// Block-scoped. Must be initialised at declaration. Cannot be reassigned.
// NOTE: for objects and arrays, the CONTENTS can still be mutated.

const PI = 3.14159;
// PI = 3; // TypeError

const user = { name: "Alice" };
user.name = "Bob";  // ✅ OK — mutating the object, not reassigning the binding
// user = {};       // ❌ TypeError — reassigning the binding

// ─── Rule of thumb ──────────────────────────────────────────────────────────
// 1. Default to const
// 2. Use let when you need to reassign
// 3. Never use var
```

**Scope types:**

| Scope | Created by | Accessible from |
|---|---|---|
| Global | Top-level declarations | Everywhere |
| Function | `function` keyword | Inside that function only |
| Block | `{ }`, `if`, `for`, etc. | Inside that block only (let/const) |
| Module | ES Module file | Inside that module only |

---

### 2. Data Types & Type Coercion

JavaScript has **7 primitive types** and **1 object type**.

```javascript
// ─── Primitives (immutable, stored by value) ─────────────────────────────────
typeof undefined   // "undefined"
typeof null        // "object" ← famous bug, kept for compatibility
typeof true        // "boolean"
typeof 42          // "number"
typeof 42n         // "bigint"   (ES2020 — arbitrary precision integers)
typeof "hello"     // "string"
typeof Symbol()    // "symbol"   (unique, immutable identifier)

// ─── Object types (mutable, stored by reference) ─────────────────────────────
typeof {}          // "object"
typeof []          // "object"  ← arrays are objects!
typeof function(){} // "function" (technically an object)

// Check for arrays specifically:
Array.isArray([])  // true

// ─── Type Coercion ───────────────────────────────────────────────────────────
// JavaScript silently converts types. This is the source of many bugs.

// == (loose equality) — performs type coercion before comparing
0   == false  // true  (false → 0)
""  == false  // true  ("" → 0, false → 0)
1   == "1"    // true  ("1" → 1)
null == undefined // true (special case)
NaN == NaN    // false (NaN is never equal to anything, including itself)

// === (strict equality) — no coercion, checks type AND value
0   === false // false
1   === "1"   // false
NaN === NaN   // false (use Number.isNaN() instead)

// ✅ Always use === unless you have a very specific reason not to
Number.isNaN(NaN)     // true
Number.isFinite(1/0)  // false

// ─── Explicit conversion ─────────────────────────────────────────────────────
Number("42")      // 42
Number("")        // 0
Number("hello")   // NaN
Number(true)      // 1
Number(false)     // 0
Number(null)      // 0
Number(undefined) // NaN

String(42)        // "42"
String(null)      // "null"
String(undefined) // "undefined"

Boolean(0)        // false
Boolean("")       // false
Boolean(null)     // false
Boolean(undefined) // false
Boolean(NaN)      // false
// Everything else is truthy — including "0", "false", [], {}
```

---

### 3. Functions

Functions are **first-class citizens** in JavaScript — they can be assigned to variables, passed as arguments, and returned from other functions.

```javascript
// ─── Function Declaration ────────────────────────────────────────────────────
// Hoisted — can be called before the declaration in the source code.
greet("Alice"); // Works! Declaration is hoisted.

function greet(name) {
    return `Hello, ${name}!`;
}

// ─── Function Expression ─────────────────────────────────────────────────────
// Not hoisted — the variable is hoisted but not its value.
// sayHi("Bob"); // TypeError: sayHi is not a function

const sayHi = function(name) {
    return `Hi, ${name}!`;
};

// ─── Arrow Functions (ES6) ───────────────────────────────────────────────────
// Shorter syntax. Does NOT have its own `this`, `arguments`, or `prototype`.
// Cannot be used as constructors.

const double = (x) => x * 2;          // implicit return
const add    = (a, b) => a + b;        // multiple params
const getObj = (x) => ({ value: x });  // return object literal — needs parens

// Multi-line arrow function needs explicit return:
const multiply = (a, b) => {
    const result = a * b;
    return result;
};

// ─── Default Parameters ──────────────────────────────────────────────────────
function createUser(name, role = "viewer", active = true) {
    return { name, role, active };
}
createUser("Alice");               // { name: "Alice", role: "viewer", active: true }
createUser("Bob", "admin");        // { name: "Bob", role: "admin", active: true }
createUser("Eve", "editor", false); // { name: "Eve", role: "editor", active: false }

// ─── Rest Parameters ─────────────────────────────────────────────────────────
// Collects remaining arguments into a real array (unlike `arguments` object).
function sum(...numbers) {
    return numbers.reduce((acc, n) => acc + n, 0);
}
sum(1, 2, 3, 4, 5); // 15

// ─── IIFE (Immediately Invoked Function Expression) ──────────────────────────
// Runs immediately. Used to create a private scope (pre-modules pattern).
const counter = (function() {
    let count = 0; // private — not accessible from outside
    return {
        increment() { count++; },
        value()     { return count; }
    };
})();
counter.increment();
counter.value(); // 1
```

---

### 4. Objects

```javascript
// ─── Object Literal ──────────────────────────────────────────────────────────
const car = {
    make: "Toyota",
    model: "Corolla",
    year: 2023,

    // Method shorthand (ES6)
    describe() {
        // `this` refers to the car object when called as car.describe()
        return `${this.year} ${this.make} ${this.model}`;
    },
};

// ─── Property Shorthand ──────────────────────────────────────────────────────
const name = "Alice";
const age  = 30;
const user = { name, age }; // same as { name: name, age: age }

// ─── Computed Property Names ─────────────────────────────────────────────────
const key   = "dynamic";
const obj   = { [key]: "value" }; // { dynamic: "value" }
const field = "email";
const prefixed = { [`user_${field}`]: "alice@example.com" }; // { user_email: ... }

// ─── Object Methods ──────────────────────────────────────────────────────────
const source = { a: 1, b: 2, c: 3 };

Object.keys(source)    // ["a", "b", "c"]
Object.values(source)  // [1, 2, 3]
Object.entries(source) // [["a", 1], ["b", 2], ["c", 3]]

// Shallow copy / merge (Object.assign mutates target; spread creates new object)
const copy   = { ...source };
const merged = { ...source, d: 4, b: 99 }; // b is overwritten

// Freeze — prevents mutations (shallow)
const config = Object.freeze({ maxRetries: 3, timeout: 5000 });
// config.maxRetries = 10; // Silently fails in sloppy mode, throws in strict mode

// ─── Property Descriptors ────────────────────────────────────────────────────
// Every property has hidden flags: writable, enumerable, configurable
Object.defineProperty(car, "vin", {
    value:        "ABC123",
    writable:     false, // cannot be changed
    enumerable:   false, // won't appear in for...in or Object.keys()
    configurable: false, // cannot be deleted or redefined
});
```

---

### 5. Arrays

```javascript
const numbers = [3, 1, 4, 1, 5, 9, 2, 6];

// ─── Iteration Methods (do NOT mutate the original) ───────────────────────────

// map — transform each element, return new array of same length
const doubled = numbers.map(n => n * 2);       // [6, 2, 8, 2, 10, 18, 4, 12]

// filter — keep elements where callback returns true
const evens   = numbers.filter(n => n % 2 === 0); // [4, 2, 6]

// reduce — fold array into a single value
const sum     = numbers.reduce((acc, n) => acc + n, 0); // 31
const max     = numbers.reduce((a, b) => a > b ? a : b); // 9

// find — first element matching the condition (or undefined)
const firstBig = numbers.find(n => n > 5);     // 9

// findIndex — index of first match (or -1)
const idx      = numbers.findIndex(n => n > 5); // 5

// some — returns true if at least one element matches
const hasNegative = numbers.some(n => n < 0);  // false

// every — returns true if ALL elements match
const allPositive = numbers.every(n => n > 0); // true

// flat & flatMap
const nested = [[1, 2], [3, [4, 5]]];
nested.flat()    // [1, 2, 3, [4, 5]] — one level
nested.flat(2)   // [1, 2, 3, 4, 5]  — two levels
nested.flat(Infinity) // fully flattened

const sentences = ["hello world", "foo bar"];
sentences.flatMap(s => s.split(" ")); // ["hello", "world", "foo", "bar"]

// ─── Sorting ──────────────────────────────────────────────────────────────────
// ⚠️ Default sort converts to strings — breaks for numbers!
[10, 9, 2, 1, 100].sort()            // [1, 10, 100, 2, 9] ← WRONG for numbers
[10, 9, 2, 1, 100].sort((a, b) => a - b) // [1, 2, 9, 10, 100] ← correct

// Sort objects by property
const people = [{ name: "Zara", age: 25 }, { name: "Alice", age: 30 }];
people.sort((a, b) => a.name.localeCompare(b.name));

// ─── Mutating Methods (modify the original array) ────────────────────────────
const arr = [1, 2, 3];
arr.push(4);    // [1, 2, 3, 4] — add to end
arr.pop();      // removes and returns last element
arr.unshift(0); // [0, 1, 2, 3] — add to start
arr.shift();    // removes and returns first element
arr.splice(1, 1, 99); // at index 1, remove 1, insert 99

// ─── Creating arrays ─────────────────────────────────────────────────────────
Array.from({ length: 5 }, (_, i) => i * 2) // [0, 2, 4, 6, 8]
Array.from("hello")                          // ["h", "e", "l", "l", "o"]
[...new Set([1, 2, 2, 3, 3])]               // [1, 2, 3] — deduplicate
```

---

### 6. Destructuring

Destructuring is syntactic sugar for extracting values from objects or arrays.

```javascript
// ─── Array Destructuring ─────────────────────────────────────────────────────
const [first, second, , fourth] = [10, 20, 30, 40]; // skip with empty comma
// first = 10, second = 20, fourth = 40

// Default values
const [a = 0, b = 0] = [1]; // a = 1, b = 0

// Swap variables (no temp variable needed)
let x = 1, y = 2;
[x, y] = [y, x]; // x = 2, y = 1

// Rest in destructuring
const [head, ...tail] = [1, 2, 3, 4]; // head = 1, tail = [2, 3, 4]

// ─── Object Destructuring ────────────────────────────────────────────────────
const { name, age } = { name: "Alice", age: 30, role: "admin" };
// name = "Alice", age = 30 (role is ignored)

// Rename while destructuring
const { name: userName, age: userAge } = { name: "Bob", age: 25 };
// userName = "Bob", userAge = 25

// Default values
const { host = "localhost", port = 3000 } = { port: 8080 };
// host = "localhost", port = 8080

// Nested destructuring
const { address: { city, country } } = {
    address: { city: "Paris", country: "France" }
};

// ─── In function parameters ──────────────────────────────────────────────────
function renderUser({ name, age = 0, role = "guest" }) {
    return `${name} (${age}) — ${role}`;
}
renderUser({ name: "Alice", age: 30 }); // "Alice (30) — guest"
```

---

### 7. Spread & Rest

```javascript
// ─── Spread (...) — expands an iterable ──────────────────────────────────────

// Copy & merge arrays
const a = [1, 2, 3];
const b = [4, 5, 6];
const merged = [...a, ...b];          // [1, 2, 3, 4, 5, 6]
const copy   = [...a];                // shallow copy

// Copy & merge objects
const defaults = { theme: "dark", lang: "en" };
const overrides = { lang: "fr", size: "large" };
const config = { ...defaults, ...overrides }; // { theme: "dark", lang: "fr", size: "large" }

// Pass array elements as individual function arguments
Math.max(...[3, 1, 4, 1, 5, 9]); // 9

// ─── Rest (...) — collects remaining elements ─────────────────────────────────

// Rest in function parameters (collect remaining args into array)
function log(level, ...messages) {
    messages.forEach(msg => console.log(`[${level}]`, msg));
}
log("INFO", "Server started", "Listening on port 3000");

// Rest in destructuring
const { id, ...rest } = { id: 1, name: "Alice", role: "admin" };
// id = 1, rest = { name: "Alice", role: "admin" }
// Useful for "omit a property" pattern
```

---

### 8. Template Literals

```javascript
const name = "World";
const multiLine = `
    Hello, ${name}!
    The answer is ${40 + 2}.
    JS expression: ${[1,2,3].map(n => n * 2).join(", ")}
`;

// ─── Tagged Templates ────────────────────────────────────────────────────────
// A function called with the template's parts — used for custom processing.
// Libraries like styled-components, graphql-tag, and sql use this pattern.

function highlight(strings, ...values) {
    return strings.reduce((result, str, i) => {
        const val = values[i] !== undefined ? `<mark>${values[i]}</mark>` : "";
        return result + str + val;
    }, "");
}

const user = "Alice";
const score = 98;
highlight`Player ${user} scored ${score} points.`;
// "Player <mark>Alice</mark> scored <mark>98</mark> points."

// ─── String.raw ──────────────────────────────────────────────────────────────
// Built-in tag — preserves raw backslashes (useful for regex, file paths)
String.raw`C:\Users\Alice\Documents`; // "C:\Users\Alice\Documents" (no escape)
```

---

### 9. Classes & Prototypes

```javascript
// ─── Class syntax (ES6+) ─────────────────────────────────────────────────────
// Classes in JavaScript are syntactic sugar over prototype-based inheritance.
// Under the hood, it's still the prototype chain.

class Animal {
    // Private field (ES2022) — truly private, not accessible outside the class
    #name;
    #sound;

    constructor(name, sound) {
        this.#name  = name;
        this.#sound = sound;
    }

    // Getter — access like a property: animal.name
    get name() { return this.#name; }

    speak() {
        return `${this.#name} says ${this.#sound}!`;
    }

    // Static method — called on the class, not on instances
    static kingdom() { return "Animalia"; }
}

class Dog extends Animal {
    #tricks = []; // initialised to empty array

    constructor(name) {
        super(name, "Woof"); // must call super() before using `this`
    }

    learn(trick) {
        this.#tricks.push(trick);
    }

    get knownTricks() {
        return [...this.#tricks]; // return a copy, not the internal array
    }

    // Override parent method
    speak() {
        const base = super.speak();
        return `${base} (tail wagging)`;
    }
}

const dog = new Dog("Rex");
dog.speak();        // "Rex says Woof! (tail wagging)"
Animal.kingdom();   // "Animalia"
// dog.#tricks      // SyntaxError — truly private

// ─── The Prototype Chain ─────────────────────────────────────────────────────
// When you access a property, JS looks up the chain until found or undefined.
// dog → Dog.prototype → Animal.prototype → Object.prototype → null

// ─── `this` — the most misunderstood thing in JavaScript ────────────────────
// `this` depends on HOW a function is called, not where it's defined.

const obj = {
    value: 42,
    getValue() { return this.value; },        // this = obj when called as obj.getValue()
    getValueArrow: () => this,                // Arrow: this = enclosing scope (NOT obj!)
};

const fn = obj.getValue;
fn();         // undefined (or global in non-strict) — `this` is lost!

// Fix: bind
const bound = obj.getValue.bind(obj);
bound();      // 42

// Fix: arrow function in class callbacks
class Timer {
    #seconds = 0;
    start() {
        setInterval(() => { this.#seconds++; }, 1000); // arrow preserves `this`
    }
}
```

---

### 10. Closures & Scope

A closure is a function that **remembers the variables from its outer scope** even after that scope has finished executing.

```javascript
// ─── Basic closure ───────────────────────────────────────────────────────────
function makeCounter(start = 0) {
    let count = start; // captured in the closure

    return {
        increment() { count++; },
        decrement() { count--; },
        value()     { return count; },
        reset()     { count = start; },
    };
}

const counter = makeCounter(10);
counter.increment();
counter.increment();
counter.value(); // 12
// `count` is not accessible from outside — it's truly encapsulated

// ─── Factory function pattern ─────────────────────────────────────────────────
function makeMultiplier(factor) {
    return (n) => n * factor; // `factor` is captured
}

const double = makeMultiplier(2);
const triple = makeMultiplier(3);
double(5); // 10
triple(5); // 15

// ─── Classic closure pitfall ─────────────────────────────────────────────────
// With var, all closures share the same variable (function scope):
const funcs_bad = [];
for (var i = 0; i < 3; i++) {
    funcs_bad.push(() => i); // all capture the same `i` !
}
funcs_bad[0](); // 3 — not 0!

// Fix: use let (block-scoped, new binding per iteration):
const funcs_good = [];
for (let i = 0; i < 3; i++) {
    funcs_good.push(() => i); // each has its own `i`
}
funcs_good[0](); // 0 ✅
funcs_good[1](); // 1 ✅
```

---

### 11. Higher-Order Functions

A higher-order function takes a function as an argument **or** returns a function.

```javascript
// ─── Function as argument (callback) ─────────────────────────────────────────
function applyTwice(fn, value) {
    return fn(fn(value));
}
applyTwice(x => x * 2, 3); // 12

// ─── Function composition ─────────────────────────────────────────────────────
const compose = (...fns) => x => fns.reduceRight((v, f) => f(v), x);
const pipe    = (...fns) => x => fns.reduce((v, f) => f(v), x);

const addOne   = x => x + 1;
const double   = x => x * 2;
const toString = x => `Result: ${x}`;

const transform = pipe(addOne, double, toString);
transform(4); // "Result: 10"   (4+1=5, 5*2=10)

// ─── Memoization ─────────────────────────────────────────────────────────────
// Cache expensive function results
function memoize(fn) {
    const cache = new Map();
    return function(...args) {
        const key = JSON.stringify(args);
        if (cache.has(key)) return cache.get(key);
        const result = fn.apply(this, args);
        cache.set(key, result);
        return result;
    };
}

const expensiveCalc = memoize((n) => {
    console.log(`Computing for ${n}...`);
    return n * n;
});
expensiveCalc(5); // logs + computes
expensiveCalc(5); // returns from cache — no log
```

---

### 12. Promises

Promises represent a value that will be available **in the future** (or an error if it fails).

```javascript
// ─── Creating a Promise ───────────────────────────────────────────────────────
const fetchUser = (id) => new Promise((resolve, reject) => {
    if (id <= 0) {
        reject(new Error(`Invalid ID: ${id}`));
        return;
    }
    setTimeout(() => {
        resolve({ id, name: "Alice" });
    }, 500);
});

// ─── Consuming with .then/.catch/.finally ────────────────────────────────────
fetchUser(1)
    .then(user => {
        console.log("Got user:", user);
        return user.name; // chain: the next .then receives this value
    })
    .then(name => console.log("Name:", name))
    .catch(err => console.error("Error:", err.message))
    .finally(() => console.log("Done — always runs"));

// ─── Combinators ─────────────────────────────────────────────────────────────

// Promise.all — all must succeed; fails fast if any reject
Promise.all([fetchUser(1), fetchUser(2)])
    .then(([user1, user2]) => console.log(user1, user2));

// Promise.allSettled — waits for all, gives you each outcome
Promise.allSettled([fetchUser(1), fetchUser(-1)])
    .then(results => {
        results.forEach(r => {
            if (r.status === "fulfilled") console.log("✅", r.value);
            else                          console.log("❌", r.reason.message);
        });
    });

// Promise.race — first to settle wins (resolve OR reject)
const timeout = new Promise((_, reject) =>
    setTimeout(() => reject(new Error("Timeout")), 3000)
);
Promise.race([fetchUser(1), timeout]).then(console.log).catch(console.error);

// Promise.any — first to RESOLVE wins (ignores rejections unless all fail)
Promise.any([fetchUser(-1), fetchUser(1)]).then(console.log);
```

---

### 13. Async / Await

Syntactic sugar over Promises. Makes async code read like synchronous code.

```javascript
// ─── Basic async/await ───────────────────────────────────────────────────────
async function loadUserData(id) {
    try {
        const user    = await fetchUser(id);        // waits for promise
        const profile = await fetchProfile(user.id); // sequential — waits for previous
        return { user, profile };
    } catch (err) {
        console.error("Failed to load:", err.message);
        throw err; // re-throw so caller can handle
    } finally {
        console.log("loadUserData finished");
    }
}

// ─── Sequential vs Parallel ─────────────────────────────────────────────────
async function example() {
    // ❌ Sequential — takes 1000ms (500 + 500)
    const user1 = await fetchUser(1);
    const user2 = await fetchUser(2);

    // ✅ Parallel — takes ~500ms (both run at the same time)
    const [user1p, user2p] = await Promise.all([fetchUser(1), fetchUser(2)]);
}

// ─── Async iteration ─────────────────────────────────────────────────────────
async function* generatePages(baseUrl) {
    let page = 1;
    while (true) {
        const data = await fetch(`${baseUrl}?page=${page}`).then(r => r.json());
        if (data.length === 0) break;
        yield data;
        page++;
    }
}

for await (const page of generatePages("/api/items")) {
    console.log("Page items:", page);
}
```

---

### 14. Fetch API

```javascript
// ─── GET ─────────────────────────────────────────────────────────────────────
async function getUser(id) {
    const response = await fetch(`/api/users/${id}`);

    // ⚠️ fetch() only rejects on network errors, NOT on 4xx/5xx responses!
    // Always check response.ok
    if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
    }

    return response.json(); // parses JSON body, returns Promise
}

// ─── POST with JSON body ──────────────────────────────────────────────────────
async function createUser(userData) {
    const response = await fetch("/api/users", {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
            "Authorization": `Bearer ${getToken()}`,
        },
        body: JSON.stringify(userData),
    });

    if (!response.ok) throw new Error(`Failed: ${response.status}`);
    return response.json();
}

// ─── AbortController — cancel in-flight requests ──────────────────────────────
async function fetchWithTimeout(url, ms = 5000) {
    const controller = new AbortController();
    const timeoutId  = setTimeout(() => controller.abort(), ms);

    try {
        const response = await fetch(url, { signal: controller.signal });
        clearTimeout(timeoutId);
        if (!response.ok) throw new Error(`HTTP ${response.status}`);
        return response.json();
    } catch (err) {
        if (err.name === "AbortError") throw new Error(`Request timed out after ${ms}ms`);
        throw err;
    }
}
```

---

### 15. DOM Manipulation

```javascript
// ─── Selecting Elements ──────────────────────────────────────────────────────
const el      = document.querySelector("#my-id");        // first match
const all     = document.querySelectorAll(".my-class");  // NodeList (not array!)
const allArr  = Array.from(document.querySelectorAll("p")); // convert to array

// ─── Creating and Inserting ──────────────────────────────────────────────────
const div = document.createElement("div");
div.textContent = "Hello";             // ✅ safe — no HTML parsing
div.className   = "card";
div.dataset.id  = "42";               // → <div data-id="42">

// Insertion methods
parent.append(div);           // adds at end (accepts strings too)
parent.prepend(div);          // adds at start
parent.before(div);           // adds before parent
parent.after(div);            // adds after parent
parent.replaceWith(div);      // replaces parent

// ─── Modifying Elements ──────────────────────────────────────────────────────
el.textContent = "New text";           // ✅ safe
el.innerHTML   = "<b>Bold</b>";        // ⚠️ XSS risk — never use with user input!

el.classList.add("active");
el.classList.remove("hidden");
el.classList.toggle("open");
el.classList.contains("active"); // true/false
el.classList.replace("old", "new");

el.setAttribute("aria-label", "Close");
el.getAttribute("data-id");
el.removeAttribute("disabled");

// ─── Reading dimensions ──────────────────────────────────────────────────────
const rect = el.getBoundingClientRect(); // { top, left, width, height, ... }
el.offsetWidth;   // layout width (no margin)
el.scrollTop;     // scroll position

// ─── Document Fragment — batch DOM updates for performance ───────────────────
const fragment = document.createDocumentFragment();
items.forEach(item => {
    const li = document.createElement("li");
    li.textContent = item;
    fragment.append(li);
});
list.append(fragment); // single DOM update — much faster than many appends
```

---

### 16. Events

```javascript
// ─── addEventListener ─────────────────────────────────────────────────────────
const btn = document.querySelector("#my-btn");

btn.addEventListener("click", handleClick);
btn.removeEventListener("click", handleClick); // must pass same function reference!

function handleClick(event) {
    event.preventDefault();   // stop default action (form submit, link follow)
    event.stopPropagation();  // stop bubbling up to parent elements
    console.log("Clicked:", event.target); // element that was clicked
    console.log("Current:", event.currentTarget); // element listener is attached to
}

// ─── Event Delegation ────────────────────────────────────────────────────────
// Instead of attaching to every child, attach ONE listener to the parent.
// Works for dynamically added elements too.
document.querySelector("#list").addEventListener("click", (event) => {
    const item = event.target.closest("li"); // walk up to find matching ancestor
    if (!item) return; // click wasn't on an li

    const id = item.dataset.id;
    console.log("Clicked item:", id);
});

// ─── Custom Events ────────────────────────────────────────────────────────────
const myEvent = new CustomEvent("user-login", {
    detail:  { userId: 42 },
    bubbles: true,
});
document.dispatchEvent(myEvent);
document.addEventListener("user-login", (e) => console.log(e.detail.userId));

// ─── Common Event Types ───────────────────────────────────────────────────────
// Mouse:    click, dblclick, mouseenter, mouseleave, mousemove, contextmenu
// Keyboard: keydown, keyup (use event.key, not event.keyCode — deprecated)
// Form:     submit, input, change, focus, blur, focusin, focusout
// Window:   load, DOMContentLoaded, resize, scroll, beforeunload
// Touch:    touchstart, touchmove, touchend
// Drag:     dragstart, dragover, drop
// Observers: IntersectionObserver, MutationObserver, ResizeObserver
```

---

### 17. Error Handling

```javascript
// ─── try / catch / finally ────────────────────────────────────────────────────
function parseJSON(str) {
    try {
        return JSON.parse(str);
    } catch (err) {
        // err.name, err.message, err.stack
        console.error(`JSON parse failed: ${err.message}`);
        return null;
    } finally {
        // Always runs — even if try returns or catch re-throws
        console.log("parseJSON complete");
    }
}

// ─── Custom Error Classes ─────────────────────────────────────────────────────
class AppError extends Error {
    constructor(message, code) {
        super(message);        // sets this.message
        this.name  = "AppError";
        this.code  = code;
        // Capture stack trace (V8/Node.js)
        if (Error.captureStackTrace) {
            Error.captureStackTrace(this, AppError);
        }
    }
}

class ValidationError extends AppError {
    constructor(field, message) {
        super(message, "VALIDATION_ERROR");
        this.name  = "ValidationError";
        this.field = field;
    }
}

try {
    throw new ValidationError("email", "Invalid email format");
} catch (err) {
    if (err instanceof ValidationError) {
        console.error(`Field '${err.field}': ${err.message}`);
    } else if (err instanceof AppError) {
        console.error(`App error [${err.code}]: ${err.message}`);
    } else {
        throw err; // re-throw unknown errors
    }
}
```

---

### 18. Optional Chaining & Nullish Coalescing

```javascript
// ─── Optional Chaining (?.) ───────────────────────────────────────────────────
// Returns undefined instead of throwing if a property doesn't exist.
const user = { profile: { address: { city: "Paris" } } };

user.profile?.address?.city;  // "Paris"
user.contact?.phone;          // undefined (no throw)
user.settings?.theme?.color;  // undefined

// Works with methods and array access too
user.getName?.();             // calls only if exists, otherwise undefined
arr?.[0];                     // undefined if arr is null/undefined

// ─── Nullish Coalescing (??) ──────────────────────────────────────────────────
// Returns right side only if left side is null or undefined.
// Unlike ||, it does NOT trigger for 0, "", or false.

const port = config.port ?? 3000;  // 3000 only if config.port is null/undefined
const name = user.name ?? "Guest"; // "Guest" only if null/undefined

// ⚠️ The OR trap
const port2 = config.port || 3000; // 3000 if config.port is 0 (falsy!) — WRONG
// If the user explicitly set port to 0, || would incorrectly use the default.
// ?? is the correct choice for "use default only if not provided"

// ─── Nullish Assignment (??=) ─────────────────────────────────────────────────
user.role ??= "viewer"; // assigns "viewer" only if user.role is null/undefined

// ─── Logical Assignment ───────────────────────────────────────────────────────
a ||= b;  // a = a || b — assign b if a is falsy
a &&= b;  // a = a && b — assign b if a is truthy
a ??= b;  // a = a ?? b — assign b if a is null/undefined
```

---

### 19. Modules (ES Modules)

```javascript
// ─── Exporting ───────────────────────────────────────────────────────────────

// math.js
export const PI = 3.14159;

export function add(a, b) { return a + b; }

export class Vector {
    constructor(x, y) { this.x = x; this.y = y; }
    magnitude() { return Math.sqrt(this.x ** 2 + this.y ** 2); }
}

// Default export — one per file
export default class Calculator { /* ... */ }

// ─── Importing ───────────────────────────────────────────────────────────────

// Named imports
import { PI, add, Vector } from "./math.js";

// Rename on import
import { add as sum } from "./math.js";

// Default import (any name works)
import Calculator from "./math.js";

// Import everything as namespace
import * as Math from "./math.js";
Math.add(1, 2);
Math.PI;

// Dynamic import — loads module on demand (lazy loading)
const { add } = await import("./math.js");

// ─── In HTML ─────────────────────────────────────────────────────────────────
// Use type="module" to enable ES modules in browsers
// <script type="module" src="main.js"></script>

// Module characteristics:
// - Always in strict mode
// - Has its own scope (variables don't leak to global)
// - Executed once per page (cached after first import)
// - Supports top-level await
```

---

### 20. Storage (localStorage / sessionStorage)

```javascript
// ─── localStorage — persists until explicitly cleared ─────────────────────────
localStorage.setItem("theme", "dark");
localStorage.getItem("theme");         // "dark"
localStorage.removeItem("theme");
localStorage.clear();                  // removes ALL items

// ⚠️ Only stores strings — always serialize objects
const user = { name: "Alice", prefs: { lang: "fr" } };
localStorage.setItem("user", JSON.stringify(user));
const restored = JSON.parse(localStorage.getItem("user") ?? "null");

// ─── sessionStorage — cleared when tab closes ────────────────────────────────
// Same API as localStorage, but only lives for the browser session

// ─── Utility wrapper with error handling ─────────────────────────────────────
const store = {
    get(key, fallback = null) {
        try {
            const item = localStorage.getItem(key);
            return item !== null ? JSON.parse(item) : fallback;
        } catch { return fallback; }
    },
    set(key, value) {
        try {
            localStorage.setItem(key, JSON.stringify(value));
        } catch (err) {
            // QuotaExceededError — storage is full
            console.error("Storage write failed:", err);
        }
    },
    remove(key) { localStorage.removeItem(key); },
};
```

---

### 21. Security Pitfalls

These are the things that get applications hacked. Understand them — every one of them.

```javascript
// ─── 1. XSS via innerHTML ─────────────────────────────────────────────────────
// ❌ DANGEROUS — if userInput contains <script>...</script>, it executes
element.innerHTML = userInput;
element.innerHTML = `<p>Welcome, ${userName}!</p>`;

// ✅ SAFE — textContent treats content as text, never HTML
element.textContent = userInput;

// ✅ SAFE — if you need to render some HTML, sanitize first
import DOMPurify from "dompurify";
element.innerHTML = DOMPurify.sanitize(userInput);

// ─── 2. eval() ────────────────────────────────────────────────────────────────
// ❌ DANGEROUS — executes arbitrary JavaScript strings
eval(userInput); // Remote Code Execution waiting to happen
setTimeout(userInput, 0); // same problem — setTimeout accepts strings!
new Function(userInput)(); // same problem

// ✅ SAFE — use proper data structures, never eval user input
// If you're using eval, you're doing something wrong.

// ─── 3. Prototype Pollution ──────────────────────────────────────────────────
// ❌ DANGEROUS — merging user-controlled objects can pollute Object.prototype
function merge(target, source) {
    for (const key in source) {
        target[key] = source[key]; // if key is "__proto__", this pollutes!
    }
}
const malicious = JSON.parse('{"__proto__": {"isAdmin": true}}');
merge({}, malicious);
({}).isAdmin; // true — every object in the app is now "admin"!

// ✅ SAFE — check for dangerous keys, or use Object.create(null) for maps
function safeMerge(target, source) {
    for (const key of Object.keys(source)) { // own keys only, no prototype
        if (key === "__proto__" || key === "constructor" || key === "prototype") continue;
        target[key] = source[key];
    }
}

// ─── 4. Insecure Direct Object References ────────────────────────────────────
// ❌ DANGEROUS — trusting user-provided IDs without authorisation check
async function getDocument(id) {
    return fetch(`/api/documents/${id}`); // is the user allowed to see this?
}

// ✅ Server-side authorisation check is required — the client is never trusted

// ─── 5. Sensitive data in localStorage ───────────────────────────────────────
// ❌ DANGEROUS — accessible to any JS on the page (XSS can steal it)
localStorage.setItem("auth_token", token);
localStorage.setItem("password", password);

// ✅ Use httpOnly cookies for auth tokens (JS can't read them)
// ✅ Never store passwords client-side — ever

// ─── 6. postMessage without origin check ─────────────────────────────────────
// ❌ DANGEROUS
window.addEventListener("message", (event) => {
    processData(event.data); // any origin can send data!
});

// ✅ SAFE
window.addEventListener("message", (event) => {
    if (event.origin !== "https://trusted-origin.com") return;
    processData(event.data);
});
```

---

### 22. Performance Patterns

```javascript
// ─── Debounce — delay execution until user stops ──────────────────────────────
// Use case: search input, window resize handler
function debounce(fn, delay) {
    let timer;
    return function(...args) {
        clearTimeout(timer);
        timer = setTimeout(() => fn.apply(this, args), delay);
    };
}

const handleSearch = debounce((query) => {
    fetch(`/api/search?q=${encodeURIComponent(query)}`);
}, 300); // only fires 300ms after user stops typing

input.addEventListener("input", (e) => handleSearch(e.target.value));

// ─── Throttle — limit execution rate ─────────────────────────────────────────
// Use case: scroll handler, mousemove, real-time updates
function throttle(fn, limit) {
    let lastCall = 0;
    return function(...args) {
        const now = Date.now();
        if (now - lastCall >= limit) {
            lastCall = now;
            fn.apply(this, args);
        }
    };
}

const handleScroll = throttle(() => {
    // runs at most once per 100ms
    updateScrollPosition();
}, 100);

window.addEventListener("scroll", handleScroll);

// ─── Lazy Loading ─────────────────────────────────────────────────────────────
// Load resources only when needed
const LazyModule = {
    _module: null,
    async load() {
        this._module ??= await import("./heavy-module.js");
        return this._module;
    }
};

// ─── IntersectionObserver — efficient scroll-based effects ────────────────────
const observer = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
        if (entry.isIntersecting) {
            entry.target.classList.add("visible");
            observer.unobserve(entry.target); // stop watching after first trigger
        }
    });
}, { threshold: 0.1 }); // 10% visible triggers callback

document.querySelectorAll(".fade-in").forEach(el => observer.observe(el));

// ─── Web Workers — offload heavy computation ──────────────────────────────────
// Runs in a separate thread — doesn't block the UI
const worker = new Worker("./worker.js");
worker.postMessage({ task: "computePrimes", limit: 1_000_000 });
worker.onmessage = (e) => console.log("Primes found:", e.data.count);
```

---

## 🇫🇷 Français

### Résumé des concepts clés

Ce document est la référence anglaise complète de JavaScript. Voici les points essentiels à retenir, résumés en français, pour les développeurs francophones.

---

#### Variables & Portée

- **`const`** par défaut. **`let`** quand la valeur change. Ne jamais utiliser **`var`**.
- `var` est à portée de fonction et sujet au *hoisting* — source de bugs imprévisibles.
- `let` et `const` ont une **Temporal Dead Zone** (TDZ) : y accéder avant leur déclaration lève une `ReferenceError`.

#### Types & Coercition

- JavaScript convertit silencieusement les types. C'est dangereux.
- `==` fait de la coercition. `===` vérifie le type **et** la valeur. **Toujours utiliser `===`**.
- `typeof null === "object"` est un bug historique conservé pour la compatibilité.

#### Fonctions

- Les fonctions fléchées (`=>`) n'ont **pas** de `this` propre — elles héritent du `this` de leur contexte.
- La valeur de `this` dans une fonction classique dépend de **comment** elle est appelée, pas où elle est définie.

#### Prototypes & Classes

- Les classes JavaScript sont du sucre syntaxique sur la **chaîne de prototypes**.
- Les champs privés (`#champ`) sont **vraiment** privés — ce n'est pas juste une convention.
- Toujours appeler `super()` avant d'utiliser `this` dans un constructeur enfant.

#### Asynchrone

- `fetch()` ne rejette **pas** sur les erreurs HTTP (404, 500). Toujours vérifier `response.ok`.
- Préférer **`Promise.all`** pour les requêtes parallèles plutôt que `await` successifs.
- Un `async function` retourne toujours une Promise.

#### Sécurité

- **Ne jamais** utiliser `innerHTML` avec des données utilisateur → XSS.
- **Ne jamais** utiliser `eval()` → exécution de code arbitraire.
- **Ne jamais** stocker des tokens d'authentification dans `localStorage` → lisibles par tout script.
- **Ne jamais** faire confiance aux données côté client — toujours valider côté serveur.

#### Performance

- **Debounce** : retarder l'exécution jusqu'à ce que l'utilisateur s'arrête (champ de recherche).
- **Throttle** : limiter la fréquence d'exécution (scroll, mousemove).
- **IntersectionObserver** : détecter la visibilité sans écouter `scroll` — bien plus performant.
- **DocumentFragment** : grouper les modifications du DOM en une seule opération.

---

## License

MIT — part of the [CodeSamples](https://github.com/t.orbeck/codesamples) project.
