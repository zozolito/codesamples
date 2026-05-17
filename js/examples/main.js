/**
 * main.js — JavaScript Examples
 * CodeSamples · https://github.com/t.orbeck/codesamples
 *
 * This file is the companion to index.html.
 * Every demo function runs real JavaScript and writes output to the page.
 *
 * READING GUIDE
 * ─────────────
 * Each section is clearly marked with a header comment.
 * The code demonstrates the concept, then logs results to the page panel.
 * Open browser DevTools (F12 → Console) for additional output.
 *
 * HOW DEMOS ARE WIRED
 * ────────────────────
 * Buttons with data-demo="<name>" call the matching demo function.
 * The output-panel with id="out-<name>" receives the results.
 * The `log()` helper appends formatted lines to the active panel.
 */

"use strict";

// ─────────────────────────────────────────────────────────────────────────────
// UTILITY: Output helpers
// A small helper object to write formatted output to the panels.
// This is NOT the subject of the tutorial — it's just plumbing.
// ─────────────────────────────────────────────────────────────────────────────

let _currentPanel = null;

/**
 * Set the active output panel by its DOM id.
 * All subsequent log() calls write to this panel.
 */
function setPanel(id) {
    _currentPanel = document.getElementById(id);
    if (_currentPanel) _currentPanel.innerHTML = "";
}

/**
 * Append a line to the current panel.
 * @param {string} label   — left-hand label (the expression)
 * @param {*}      value   — the result to display
 * @param {string} [type]  — "result", "error", "warn", "heading", "divider"
 */
function log(label, value, type = "result") {
    if (!_currentPanel) return;

    const line = document.createElement("div");
    line.className = `log-line log-${type}`;

    if (type === "heading") {
        line.textContent = label;
    } else if (type === "divider") {
        line.innerHTML = "<hr>";
    } else {
        const labelEl  = document.createElement("span");
        const arrowEl  = document.createElement("span");
        const valueEl  = document.createElement("span");

        labelEl.className = "log-label";
        arrowEl.className = "log-arrow";
        valueEl.className = "log-value";

        labelEl.textContent = label;
        arrowEl.textContent = " → ";
        valueEl.textContent = formatValue(value);

        line.append(labelEl, arrowEl, valueEl);
    }

    _currentPanel.append(line);
}

function formatValue(v) {
    if (v === null)      return "null";
    if (v === undefined) return "undefined";
    if (typeof v === "string") return `"${v}"`;
    if (Array.isArray(v)) return `[${v.map(formatValue).join(", ")}]`;
    if (typeof v === "object") {
        try {
            return JSON.stringify(v);
        } catch {
            return String(v);
        }
    }
    return String(v);
}


// ─────────────────────────────────────────────────────────────────────────────
// § 1 — VARIABLES & SCOPE
// ─────────────────────────────────────────────────────────────────────────────

function demoVariables() {
    setPanel("out-variables");

    // ── const ─────────────────────────────────────────────────────────────────
    // The binding cannot be reassigned, but the VALUE (object/array) can mutate.
    log("heading", "const");

    const PI = 3.14159;
    log("const PI = 3.14159; PI", PI);

    const user = { name: "Alice" };
    user.name = "Bob"; // ✅ mutating the object is fine
    log("user = { name:'Alice' }; user.name = 'Bob'; user", user);

    // ── let ───────────────────────────────────────────────────────────────────
    log("heading", "let");

    let score = 0;
    score += 10;
    score += 5;
    log("let score = 0; score += 10; score += 5; score", score);

    // ── var: hoisting ─────────────────────────────────────────────────────────
    // var is function-scoped and hoisted with value undefined.
    log("heading", "var hoisting (why var is dangerous)");

    function demoVar() {
        // Here, `x` is hoisted — it exists but is undefined.
        const before = (x === undefined) ? "undefined (hoisted!)" : x;
        var x = 5;
        if (true) {
            var x = 99; // Same variable — not a new block-scoped binding!
        }
        return { before, after: x };
    }
    const varResult = demoVar();
    log("x before declaration", varResult.before);
    log("x after if-block (var leaks through block)", varResult.after);

    // ── Block scope with let ───────────────────────────────────────────────────
    log("heading", "let — block scope");

    let outer = "outside";
    {
        let inner = "inside";
        outer = "modified from inside"; // outer is accessible
        log("inside block: inner", inner);
        log("inside block: outer", outer);
    }
    // `inner` is not accessible here — would throw ReferenceError
    log("outside block: outer", outer);
    log("outside block: inner", "→ ReferenceError (block-scoped, not shown)");

    // ── Temporal Dead Zone ─────────────────────────────────────────────────────
    log("heading", "Temporal Dead Zone (TDZ)");
    log("accessing let before declaration", "→ ReferenceError: Cannot access 'y' before initialisation");
    log("(var would give undefined instead)", "that's why var causes subtle bugs");
}


// ─────────────────────────────────────────────────────────────────────────────
// § 2 — TYPES & COERCION
// ─────────────────────────────────────────────────────────────────────────────

function demoTypes() {
    setPanel("out-types");

    // ── typeof ────────────────────────────────────────────────────────────────
    log("heading", "typeof — the 7 primitive types + object");

    log("typeof undefined",      typeof undefined);
    log("typeof null",           typeof null);       // famous bug: "object"
    log("typeof true",           typeof true);
    log("typeof 42",             typeof 42);
    log("typeof 42n (BigInt)",   typeof 42n);
    log("typeof 'hello'",        typeof "hello");
    log("typeof Symbol()",       typeof Symbol());
    log("typeof {}",             typeof {});
    log("typeof []",             typeof []);         // arrays are objects
    log("typeof function(){}",   typeof function(){});

    log("Array.isArray([])",     Array.isArray([]));  // proper array check

    // ── Loose vs strict equality ───────────────────────────────────────────────
    log("heading", "== (loose) vs === (strict)");

    log("1 == '1'    (coercion)", 1 == "1");    // true  ← dangerous
    log("1 === '1'   (strict)",   1 === "1");   // false ← correct
    log("0 == false  (coercion)", 0 == false);  // true  ← surprising
    log("0 === false (strict)",   0 === false); // false
    log("null == undefined",      null == undefined); // true (special case)
    log("NaN === NaN",            NaN === NaN); // false — NaN is never equal to itself

    log("heading", "Correct NaN check");
    log("Number.isNaN(NaN)",      Number.isNaN(NaN));      // true
    log("Number.isNaN('hello')",  Number.isNaN("hello"));  // false (no coercion)
    log("isNaN('hello') [global]", isNaN("hello"));        // true — coerces! misleading

    // ── Explicit conversion ────────────────────────────────────────────────────
    log("heading", "Explicit conversion");

    log("Number('42')",       Number("42"));
    log("Number('')",         Number(""));        // 0 — surprising
    log("Number('abc')",      Number("abc"));     // NaN
    log("Number(true)",       Number(true));      // 1
    log("Number(null)",       Number(null));      // 0 — surprising
    log("Number(undefined)",  Number(undefined)); // NaN

    // ── Falsy values ───────────────────────────────────────────────────────────
    log("heading", "Falsy values — the complete list");
    const falsyValues = [false, 0, -0, 0n, "", null, undefined, NaN];
    falsyValues.forEach(v => log(`Boolean(${formatValue(v)})`, Boolean(v)));
    log("Boolean('0') — truthy!", Boolean("0")); // true! "0" is not empty
    log("Boolean([])  — truthy!", Boolean([]));  // true! empty array is truthy
    log("Boolean({})  — truthy!", Boolean({}));  // true! empty object is truthy
}


// ─────────────────────────────────────────────────────────────────────────────
// § 3 — FUNCTIONS
// ─────────────────────────────────────────────────────────────────────────────

function demoFunctions() {
    setPanel("out-functions");

    // ── Function declaration vs expression ────────────────────────────────────
    log("heading", "Declaration vs Expression");

    // Declarations are hoisted — can be called before the definition
    log("greet('Alice') — called before definition", greet("Alice"));

    function greet(name) {
        return `Hello, ${name}!`;
    }

    // Expressions are NOT hoisted
    const sayHi = function(name) {
        return `Hi, ${name}!`;
    };
    log("sayHi('Bob')", sayHi("Bob"));

    // ── Arrow functions ────────────────────────────────────────────────────────
    log("heading", "Arrow functions");

    const double   = x => x * 2;
    const add      = (a, b) => a + b;
    const getObj   = x => ({ value: x }); // object literal needs parens

    log("double(7)",      double(7));
    log("add(3, 4)",      add(3, 4));
    log("getObj(99)",     getObj(99));

    // ── Default parameters ─────────────────────────────────────────────────────
    log("heading", "Default parameters");

    function createUser(name, role = "viewer", active = true) {
        return { name, role, active };
    }
    log("createUser('Alice')",                createUser("Alice"));
    log("createUser('Bob', 'admin')",         createUser("Bob", "admin"));
    log("createUser('Eve', 'editor', false)", createUser("Eve", "editor", false));

    // ── Rest parameters ────────────────────────────────────────────────────────
    log("heading", "Rest parameters");

    function sum(label, ...numbers) {
        const total = numbers.reduce((acc, n) => acc + n, 0);
        return `${label}: ${total}`;
    }
    log("sum('Total', 1,2,3,4,5)", sum("Total", 1, 2, 3, 4, 5));

    // ── IIFE ───────────────────────────────────────────────────────────────────
    log("heading", "IIFE — private scope");

    const counter = (function() {
        let count = 0; // private — not accessible outside
        return {
            increment() { count++; },
            value()     { return count; },
        };
    })();

    counter.increment();
    counter.increment();
    counter.increment();
    log("counter.value() after 3 increments", counter.value());
    log("'count' variable is private", "→ cannot access from outside");
}


// ─────────────────────────────────────────────────────────────────────────────
// § 4 — OBJECTS
// ─────────────────────────────────────────────────────────────────────────────

function demoObjects() {
    setPanel("out-objects");

    // ── Object literal + methods ────────────────────────────────────────────
    log("heading", "Object literal");

    const car = {
        make:  "Toyota",
        model: "Corolla",
        year:  2023,
        describe() { // method shorthand — `this` refers to `car` when called correctly
            return `${this.year} ${this.make} ${this.model}`;
        },
    };
    log("car.describe()", car.describe());

    // ── Shorthand properties ────────────────────────────────────────────────
    log("heading", "Property shorthand");

    const name = "Alice";
    const age  = 30;
    const user = { name, age }; // same as { name: name, age: age }
    log("{ name, age } where name='Alice', age=30", user);

    // ── Computed property names ─────────────────────────────────────────────
    log("heading", "Computed property names");

    const field = "email";
    const record = { [`user_${field}`]: "alice@example.com" };
    log("[`user_${field}`]: '...'", record);

    // ── Object methods ───────────────────────────────────────────────────────
    log("heading", "Object.keys / values / entries");

    const source = { a: 1, b: 2, c: 3 };
    log("Object.keys(source)",    Object.keys(source));
    log("Object.values(source)",  Object.values(source));
    log("Object.entries(source)", Object.entries(source));

    // ── Spread / merge ───────────────────────────────────────────────────────
    log("heading", "Spread copy & merge");

    const defaults  = { theme: "dark", lang: "en", size: "md" };
    const overrides = { lang: "fr", size: "lg" };
    const config    = { ...defaults, ...overrides };
    log("{ ...defaults, ...overrides }", config);

    // ── Object.freeze ────────────────────────────────────────────────────────
    log("heading", "Object.freeze — prevents mutations");

    const frozen = Object.freeze({ maxRetries: 3, timeout: 5000 });
    frozen.maxRetries = 99; // silently ignored (or throws in strict mode)
    log("frozen.maxRetries after assignment attempt", frozen.maxRetries); // still 3
}


// ─────────────────────────────────────────────────────────────────────────────
// § 5 — ARRAYS
// ─────────────────────────────────────────────────────────────────────────────

function demoArrays() {
    setPanel("out-arrays");

    const numbers = [3, 1, 4, 1, 5, 9, 2, 6];
    log("input", numbers);

    // ── map ─────────────────────────────────────────────────────────────────
    log("heading", "map — transform, returns new array of same length");
    log("numbers.map(n => n * 2)", numbers.map(n => n * 2));

    // ── filter ───────────────────────────────────────────────────────────────
    log("heading", "filter — keep where condition is true");
    log("numbers.filter(n => n % 2 === 0)", numbers.filter(n => n % 2 === 0));
    log("numbers.filter(n => n > 4)",       numbers.filter(n => n > 4));

    // ── reduce ───────────────────────────────────────────────────────────────
    log("heading", "reduce — fold into one value");
    log("numbers.reduce(sum)",  numbers.reduce((acc, n) => acc + n, 0));
    log("numbers.reduce(max)",  numbers.reduce((a, b) => a > b ? a : b));

    // ── find / findIndex ─────────────────────────────────────────────────────
    log("heading", "find / findIndex — first match");
    log("numbers.find(n => n > 5)",      numbers.find(n => n > 5));
    log("numbers.findIndex(n => n > 5)", numbers.findIndex(n => n > 5));

    // ── some / every ─────────────────────────────────────────────────────────
    log("heading", "some / every — boolean checks");
    log("numbers.some(n => n > 8)",  numbers.some(n => n > 8));
    log("numbers.every(n => n > 0)", numbers.every(n => n > 0));
    log("numbers.every(n => n > 5)", numbers.every(n => n > 5));

    // ── Sort pitfall ─────────────────────────────────────────────────────────
    log("heading", "⚠️ Sort pitfall — default sort uses string comparison");
    const nums = [10, 9, 2, 1, 100];
    log("[10,9,2,1,100].sort()              (WRONG)", [...nums].sort());
    log("[10,9,2,1,100].sort((a,b) => a-b) (correct)", [...nums].sort((a, b) => a - b));

    // ── flat / flatMap ────────────────────────────────────────────────────────
    log("heading", "flat / flatMap");
    const nested = [[1, 2], [3, [4, 5]]];
    log("[[1,2],[3,[4,5]]].flat()",    nested.flat());
    log("[[1,2],[3,[4,5]]].flat(2)",   nested.flat(2));

    // ── Creating arrays ───────────────────────────────────────────────────────
    log("heading", "Creating arrays");
    log("Array.from({length:5}, (_,i) => i*2)",    Array.from({ length: 5 }, (_, i) => i * 2));
    log("[...new Set([1,2,2,3,3])] — deduplicate", [...new Set([1, 2, 2, 3, 3])]);
}


// ─────────────────────────────────────────────────────────────────────────────
// § 6 — DESTRUCTURING
// ─────────────────────────────────────────────────────────────────────────────

function demoDestructuring() {
    setPanel("out-destructuring");

    // ── Array destructuring ──────────────────────────────────────────────────
    log("heading", "Array destructuring");

    const [first, second, , fourth] = [10, 20, 30, 40];
    log("[first, second, , fourth] = [10,20,30,40]", { first, second, fourth });

    // Default values
    const [a = 0, b = 0] = [1]; // b uses default
    log("[a=0, b=0] = [1]", { a, b });

    // Swap without temp variable
    let x = 1, y = 2;
    [x, y] = [y, x];
    log("[x, y] = [y, x] swap", { x, y });

    // Rest in array destructuring
    const [head, ...tail] = [1, 2, 3, 4, 5];
    log("[head, ...tail] = [1,2,3,4,5]", { head, tail });

    // ── Object destructuring ─────────────────────────────────────────────────
    log("heading", "Object destructuring");

    const { name, age } = { name: "Alice", age: 30, role: "admin" };
    log("{ name, age } (role ignored)", { name, age });

    // Rename
    const { name: userName, age: userAge } = { name: "Bob", age: 25 };
    log("{ name: userName, age: userAge }", { userName, userAge });

    // Default values
    const { host = "localhost", port = 3000 } = { port: 8080 };
    log("{ host='localhost', port=3000 } = { port:8080 }", { host, port });

    // Nested
    const { address: { city, country } } = {
        address: { city: "Paris", country: "France" }
    };
    log("{ address: { city, country } } nested", { city, country });

    // ── In function parameters ────────────────────────────────────────────────
    log("heading", "Destructuring in function parameters");

    function renderUser({ name: n, age: a = 0, role = "guest" }) {
        return `${n} (${a}) — ${role}`;
    }
    log("renderUser({ name:'Alice', age:30 })", renderUser({ name: "Alice", age: 30 }));
    log("renderUser({ name:'Bob' })",           renderUser({ name: "Bob" }));
}


// ─────────────────────────────────────────────────────────────────────────────
// § 7 — SPREAD & REST
// ─────────────────────────────────────────────────────────────────────────────

function demoSpreadRest() {
    setPanel("out-spread-rest");

    // ── Spread on arrays ─────────────────────────────────────────────────────
    log("heading", "Spread — arrays");

    const a = [1, 2, 3];
    const b = [4, 5, 6];
    log("[...a, ...b]",         [...a, ...b]);
    log("[...a]  (shallow copy)", [...a]);
    log("[...a, 99, ...b]",     [...a, 99, ...b]);

    // Math.max with spread
    log("Math.max(...[3,1,4,1,5,9])", Math.max(...[3, 1, 4, 1, 5, 9]));

    // ── Spread on objects ────────────────────────────────────────────────────
    log("heading", "Spread — objects (right side wins on duplicate keys)");

    const defaults  = { theme: "dark", lang: "en", size: "md" };
    const overrides = { lang: "fr", size: "lg" };
    log("{ ...defaults, ...overrides }", { ...defaults, ...overrides });

    // ── Rest in functions ─────────────────────────────────────────────────────
    log("heading", "Rest — collect remaining arguments");

    function logMessages(level, ...messages) {
        return messages.map(m => `[${level}] ${m}`);
    }
    log("log('INFO', 'Server started', 'Port 3000')",
        logMessages("INFO", "Server started", "Port 3000"));

    // ── Rest in destructuring ─────────────────────────────────────────────────
    log("heading", "Rest in object destructuring (omit a property)");

    const { id, ...rest } = { id: 1, name: "Alice", role: "admin" };
    log("const { id, ...rest } = { id:1, name:'Alice', role:'admin' }", null);
    log("id",   id);
    log("rest", rest);
}


// ─────────────────────────────────────────────────────────────────────────────
// § 8 — TEMPLATE LITERALS
// ─────────────────────────────────────────────────────────────────────────────

function demoTemplateLiterals() {
    setPanel("out-template-literals");

    // ── Basic interpolation ───────────────────────────────────────────────────
    log("heading", "Basic interpolation");

    const name  = "World";
    const score = 98;
    log("`Hello, ${name}!`",                 `Hello, ${name}!`);
    log("`2 + 2 = ${2 + 2}`",               `2 + 2 = ${2 + 2}`);
    log("Expression: array.join",            `${[1,2,3].map(n => n*2).join(", ")}`);
    log("`Score: ${score}/100`",             `Score: ${score}/100`);

    // ── Multi-line ─────────────────────────────────────────────────────────────
    log("heading", "Multi-line — no \\n needed");

    const multiLine = `Line 1
Line 2
Line 3`;
    log("multi-line template", multiLine);

    // ── Tagged templates ──────────────────────────────────────────────────────
    log("heading", "Tagged templates");

    // A tag function receives: array of static strings + interpolated values
    function highlight(strings, ...values) {
        return strings.reduce((result, str, i) => {
            const val = values[i] !== undefined ? `【${values[i]}】` : "";
            return result + str + val;
        }, "");
    }

    const user = "Alice";
    const pts  = 250;
    log("highlight`Player ${user} earned ${pts} points.`",
        highlight`Player ${user} earned ${pts} points.`);

    // ── String.raw ─────────────────────────────────────────────────────────────
    log("heading", "String.raw — preserves backslashes");
    log("String.raw`C:\\Users\\Alice`", String.raw`C:\Users\Alice`);
    log("String.raw`\\n is not a newline here`", String.raw`\n is not a newline here`);
}


// ─────────────────────────────────────────────────────────────────────────────
// § 9 — CLASSES
// ─────────────────────────────────────────────────────────────────────────────

function demoClasses() {
    setPanel("out-classes");

    // ── Define the class hierarchy ────────────────────────────────────────────
    class Animal {
        // Private fields (ES2022) — truly inaccessible outside the class
        #name;
        #sound;

        constructor(name, sound) {
            this.#name  = name;
            this.#sound = sound;
        }

        // Getter — accessed like a property: animal.name (no parens)
        get name() { return this.#name; }

        speak() {
            return `${this.#name} says ${this.#sound}!`;
        }

        // Static method — called on the CLASS, not on an instance
        static kingdom() { return "Animalia"; }
    }

    class Dog extends Animal {
        #tricks = []; // class field initialiser

        constructor(name) {
            // super() MUST be called before any use of `this`
            super(name, "Woof");
        }

        learn(trick) {
            this.#tricks.push(trick);
        }

        get knownTricks() {
            return [...this.#tricks]; // return a copy, not the internal array
        }

        speak() { // override
            return `${super.speak()} (tail wagging)`;
        }
    }

    // ── Use the classes ────────────────────────────────────────────────────────
    const dog = new Dog("Rex");
    dog.learn("sit");
    dog.learn("paw");
    dog.learn("roll over");

    log("dog.name (getter)",     dog.name);
    log("dog.speak()",           dog.speak());
    log("dog.knownTricks",       dog.knownTricks);
    log("Animal.kingdom()",      Animal.kingdom());

    // Private field access
    log("dog.#tricks (private)", "→ SyntaxError — truly inaccessible");

    // instanceof
    log("dog instanceof Dog",    dog instanceof Dog);
    log("dog instanceof Animal", dog instanceof Animal);

    // ── `this` — context matters ───────────────────────────────────────────────
    log("heading", "`this` — lost context demonstration");

    const obj = {
        value: 42,
        getValue() { return this.value; },
    };

    const lostFn = obj.getValue;
    log("obj.getValue()", obj.getValue()); // 42 — this = obj
    log("lostFn() — lost `this`",
        (() => { try { return lostFn() ?? "undefined (this is lost)"; } catch { return "error"; } })()
    );

    const boundFn = obj.getValue.bind(obj);
    log("obj.getValue.bind(obj)()", boundFn()); // 42 — this = obj again
}


// ─────────────────────────────────────────────────────────────────────────────
// § 10 — CLOSURES
// ─────────────────────────────────────────────────────────────────────────────

function demoClosures() {
    setPanel("out-closures");

    // ── Counter factory ────────────────────────────────────────────────────────
    log("heading", "Closure as private state — makeCounter");

    function makeCounter(start = 0) {
        let count = start; // `count` lives in the closure — not accessible outside

        return {
            increment() { count++; },
            decrement() { count--; },
            value()     { return count; },
            reset()     { count = start; },
        };
    }

    const c1 = makeCounter(10);
    const c2 = makeCounter(0);   // independent counter — own private `count`

    c1.increment();
    c1.increment();
    c2.increment();

    log("c1.value() (started at 10, +2)", c1.value()); // 12
    log("c2.value() (started at 0,  +1)", c2.value()); // 1
    log("c1 and c2 are independent",      "each has its own closed-over `count`");

    c1.reset();
    log("c1.value() after reset",         c1.value()); // 10

    // ── Factory function ──────────────────────────────────────────────────────
    log("heading", "Factory — makeMultiplier");

    function makeMultiplier(factor) {
        return (n) => n * factor; // `factor` is captured
    }

    const double = makeMultiplier(2);
    const triple = makeMultiplier(3);

    log("double(5) — factor:2", double(5));
    log("triple(5) — factor:3", triple(5));
    log("double(9)",             double(9));

    // ── Classic var pitfall in loops ──────────────────────────────────────────
    log("heading", "var in loops — all functions share same `i`");

    const fns_bad = [];
    for (var i = 0; i < 3; i++) {
        fns_bad.push(() => i); // all capture the same `i`
    }
    log("fns_bad[0]() — expected 0, got", fns_bad[0]()); // 3 ← wrong!
    log("fns_bad[1]() — expected 1, got", fns_bad[1]()); // 3 ← wrong!
    log("fns_bad[2]() — expected 2, got", fns_bad[2]()); // 3 ← wrong!

    log("heading", "let in loops — fresh binding per iteration ✅");

    const fns_good = [];
    for (let j = 0; j < 3; j++) {
        fns_good.push(() => j); // each iteration has its own `j`
    }
    log("fns_good[0]()", fns_good[0]()); // 0 ✅
    log("fns_good[1]()", fns_good[1]()); // 1 ✅
    log("fns_good[2]()", fns_good[2]()); // 2 ✅
}


// ─────────────────────────────────────────────────────────────────────────────
// § 11 — PROMISES
// ─────────────────────────────────────────────────────────────────────────────

function demoPromises() {
    setPanel("out-promises");

    log("heading", "Promises run asynchronously — results appear below as they resolve");
    log("(results will appear in the order they settle, not declaration order)", null, "warn");

    // Simulated async operation
    function delay(ms, value, fail = false) {
        return new Promise((resolve, reject) => {
            setTimeout(() => {
                if (fail) reject(new Error(`Failed after ${ms}ms`));
                else      resolve(value);
            }, ms);
        });
    }

    // ── Basic .then chain ─────────────────────────────────────────────────────
    delay(200, "hello")
        .then(val => {
            log(".then — received value", val);
            return val.toUpperCase(); // returned value flows to next .then
        })
        .then(upper => log(".then — chained, uppercased", upper))
        .catch(err  => log(".catch — error", err.message, "error"))
        .finally(()  => log(".finally — always runs", null));

    // ── Promise.all ────────────────────────────────────────────────────────────
    Promise.all([
        delay(100, "A"),
        delay(150, "B"),
        delay(50,  "C"),
    ]).then(results => log("Promise.all([100ms, 150ms, 50ms])", results));
    // resolves after ~150ms (longest), order preserved

    // ── Promise.allSettled ──────────────────────────────────────────────────
    Promise.allSettled([
        delay(100, "ok"),
        delay(80,  "fail", true),
    ]).then(results => {
        const summary = results.map(r =>
            r.status === "fulfilled"
                ? `✅ ${r.value}`
                : `❌ ${r.reason.message}`
        );
        log("Promise.allSettled", summary);
    });

    // ── Promise.race ──────────────────────────────────────────────────────────
    Promise.race([
        delay(300, "slow"),
        delay(100, "fast"),
    ]).then(winner => log("Promise.race — winner", winner));

    // ── Promise.any ───────────────────────────────────────────────────────────
    Promise.any([
        delay(50, "first rejected", true),
        delay(100, "first resolved"),
    ]).then(val => log("Promise.any — first resolve wins", val));
}


// ─────────────────────────────────────────────────────────────────────────────
// § 12 — ASYNC / AWAIT
// ─────────────────────────────────────────────────────────────────────────────

function demoAsyncAwait() {
    setPanel("out-async-await");

    function delay(ms, value) {
        return new Promise(resolve => setTimeout(() => resolve(value), ms));
    }

    async function runAll() {
        log("heading", "Basic async/await");

        // async functions always return a Promise
        async function loadItem(id) {
            const item = await delay(100, { id, name: `Item ${id}` });
            return item;
        }

        const item = await loadItem(42);
        log("await loadItem(42)", item);

        // ── Error handling ────────────────────────────────────────────────────
        log("heading", "Error handling with try/catch");

        async function mightFail(shouldFail) {
            if (shouldFail) throw new Error("Something went wrong");
            return await delay(50, "success");
        }

        try {
            const result = await mightFail(false);
            log("mightFail(false)", result);
        } catch (err) {
            log("mightFail(false) — error", err.message, "error");
        }

        try {
            const result = await mightFail(true);
            log("mightFail(true)", result);
        } catch (err) {
            log("mightFail(true) caught", err.message, "error");
        }

        // ── Sequential vs Parallel ─────────────────────────────────────────────
        log("heading", "Sequential vs Parallel");

        const t1 = performance.now();
        const seqA = await delay(100, "A");
        const seqB = await delay(100, "B"); // waits for A first
        const seqTime = Math.round(performance.now() - t1);
        log(`Sequential: [${seqA}, ${seqB}]`, `~${seqTime}ms (100+100)`);

        const t2 = performance.now();
        const [parA, parB] = await Promise.all([delay(100, "C"), delay(100, "D")]);
        const parTime = Math.round(performance.now() - t2);
        log(`Parallel:   [${parA}, ${parB}]`, `~${parTime}ms (both at once)`);
    }

    runAll().catch(err => log("runAll error", err.message, "error"));
}


// ─────────────────────────────────────────────────────────────────────────────
// § 13 — DOM MANIPULATION
// ─────────────────────────────────────────────────────────────────────────────

function setupDomDemo() {
    const target  = document.getElementById("dom-target");
    const btnBuild = document.getElementById("btn-dom-build");
    const btnClear = document.getElementById("btn-dom-clear");

    btnBuild.addEventListener("click", () => {
        target.innerHTML = ""; // clear

        // ── createElement ─────────────────────────────────────────────────────
        const h3 = document.createElement("h3");
        h3.textContent = "Built with JavaScript"; // ✅ safe — no HTML parsing

        // ── DocumentFragment — build subtree in memory, then insert once ───────
        // This is more efficient than appending one-by-one (only one reflow).
        const list = document.createElement("ul");
        list.className = "generated-list";

        const concepts = [
            "createElement — create a new DOM node",
            "textContent   — safe text insertion",
            "classList     — add, remove, toggle classes",
            "dataset       — custom data-* attributes",
            "DocumentFragment — batch DOM updates",
        ];

        const fragment = document.createDocumentFragment();
        concepts.forEach((text, i) => {
            const li = document.createElement("li");
            // textContent is always safe — it never parses HTML
            li.textContent = text;
            li.dataset.index = i;        // becomes data-index="0" etc.
            li.classList.add("list-item");
            if (i === 0) li.classList.add("highlight");
            fragment.append(li);
        });

        list.append(fragment); // single DOM update ← efficient

        // ── innerHTML — safe here because content is 100% developer-controlled ─
        // ⚠️ NEVER use innerHTML with user-supplied data — XSS risk
        const warn = document.createElement("p");
        warn.className = "dom-warning";
        warn.innerHTML = `<strong>⚠️ Note:</strong> <code>innerHTML</code> is used here
            with developer-controlled strings only. Never pass user input to <code>innerHTML</code>.`;

        // ── Batch insert ───────────────────────────────────────────────────────
        target.append(h3, list, warn);

        // ── Reading layout info ────────────────────────────────────────────────
        const rect = target.getBoundingClientRect();
        const info = document.createElement("p");
        info.className = "dom-rect";
        info.textContent = `getBoundingClientRect → width: ${Math.round(rect.width)}px`;
        target.append(info);
    });

    btnClear.addEventListener("click", () => {
        target.innerHTML = "";
        const p = document.createElement("p");
        p.className = "placeholder-text";
        p.textContent = "Click the button to build this area with JavaScript.";
        target.append(p);
    });
}


// ─────────────────────────────────────────────────────────────────────────────
// § 14 — EVENTS
// ─────────────────────────────────────────────────────────────────────────────

function setupEventsDemo() {
    const log2 = (() => {
        const container = document.getElementById("event-log");
        let count = 0;
        return function(msg) {
            count++;
            const line = document.createElement("div");
            line.className = "event-log-line";
            line.textContent = `[${count}] ${msg}`;
            container.prepend(line); // newest on top
            if (container.children.length > 10) {
                container.lastElementChild.remove(); // keep tidy
            }
        };
    })();

    // ── Click event ───────────────────────────────────────────────────────────
    const clickBtn = document.querySelector(".event-btn[data-action='click-demo']");
    clickBtn.addEventListener("click", (event) => {
        log2(`click on "${event.target.textContent}" (clientX: ${event.clientX})`);
    });

    // ── Keyboard events ────────────────────────────────────────────────────────
    // Use event.key — not deprecated keyCode
    const input = document.getElementById("event-input");
    input.addEventListener("keydown", (event) => {
        if (event.key === "Enter") {
            log2(`keydown: Enter — input value: "${event.target.value}"`);
        } else if (event.key === "Escape") {
            input.value = "";
            log2(`keydown: Escape — input cleared`);
        }
    });

    // Debounced input event — fires only after 400ms of silence
    let inputTimer;
    input.addEventListener("input", (event) => {
        clearTimeout(inputTimer);
        inputTimer = setTimeout(() => {
            if (event.target.value) {
                log2(`input (debounced, 400ms): "${event.target.value}"`);
            }
        }, 400);
    });

    // ── Event Delegation ──────────────────────────────────────────────────────
    // One listener on the parent handles clicks on all children.
    // Works for dynamically added items too.
    document.getElementById("event-list").addEventListener("click", (event) => {
        // event.target might be a child of the li — closest() walks up
        const item = event.target.closest("li");
        if (!item) return; // click was outside any li

        log2(`delegated click → li[data-id="${item.dataset.id}"] — "${item.textContent}"`);
    });

    // ── Custom Event ──────────────────────────────────────────────────────────
    const customEvent = new CustomEvent("demo-ready", {
        detail:  { timestamp: Date.now() },
        bubbles: true,
    });
    document.dispatchEvent(customEvent);
    document.addEventListener("demo-ready", (e) => {
        log2(`CustomEvent "demo-ready" received — timestamp: ${e.detail.timestamp}`);
    });
}


// ─────────────────────────────────────────────────────────────────────────────
// § 15 — ERROR HANDLING
// ─────────────────────────────────────────────────────────────────────────────

function demoErrors() {
    setPanel("out-errors");

    // ── try / catch / finally ─────────────────────────────────────────────────
    log("heading", "try / catch / finally");

    function parseJSON(str) {
        try {
            return { success: true, data: JSON.parse(str) };
        } catch (err) {
            return { success: false, error: `${err.name}: ${err.message}` };
        } finally {
            // This runs even if try returns or catch re-throws
            console.log("parseJSON: finally block ran");
        }
    }

    log("parseJSON('{\"ok\":true}')", parseJSON('{"ok":true}'));
    log("parseJSON('not json')",       parseJSON("not json"));

    // ── Custom error classes ───────────────────────────────────────────────────
    log("heading", "Custom error classes");

    class AppError extends Error {
        constructor(message, code) {
            super(message);
            this.name = "AppError";
            this.code = code;
        }
    }

    class ValidationError extends AppError {
        constructor(field, message) {
            super(message, "VALIDATION_ERROR");
            this.name  = "ValidationError";
            this.field = field;
        }
    }

    class NetworkError extends AppError {
        constructor(statusCode) {
            super(`HTTP ${statusCode}`, "NETWORK_ERROR");
            this.name       = "NetworkError";
            this.statusCode = statusCode;
        }
    }

    // Handle different error types with instanceof
    function handle(err) {
        if (err instanceof ValidationError) {
            return `ValidationError on field '${err.field}': ${err.message}`;
        }
        if (err instanceof NetworkError) {
            return `NetworkError ${err.statusCode}: ${err.message}`;
        }
        if (err instanceof AppError) {
            return `AppError [${err.code}]: ${err.message}`;
        }
        return `Unknown error: ${err.message}`;
    }

    const errors = [
        new ValidationError("email", "Invalid format"),
        new NetworkError(404),
        new AppError("Something went wrong", "GENERIC_ERROR"),
        new Error("Uncategorised"),
    ];

    errors.forEach(err => log(`handle(${err.name})`, handle(err)));

    // ── Always re-throw unknown errors ─────────────────────────────────────────
    log("heading", "Re-throwing unknown errors — best practice");
    log("Only catch errors you know how to handle", "re-throw everything else");
    log("Swallowing errors with empty catch blocks", "→ hides bugs, makes debugging hell");
}


// ─────────────────────────────────────────────────────────────────────────────
// § 16 — OPTIONAL CHAINING & NULLISH COALESCING
// ─────────────────────────────────────────────────────────────────────────────

function demoOptionalChaining() {
    setPanel("out-optional-chaining");

    // ── Optional chaining (?.) ────────────────────────────────────────────────
    log("heading", "Optional chaining (?.)");

    const user = {
        profile: { address: { city: "Paris" } },
        getName() { return "Alice"; },
    };
    const emptyUser = {};

    log("user.profile?.address?.city",     user.profile?.address?.city);
    log("user.contact?.phone",             user.contact?.phone); // undefined, no throw
    log("emptyUser.profile?.address?.city", emptyUser.profile?.address?.city);

    // Method calls
    log("user.getName?.()",               user.getName?.());
    log("emptyUser.getName?.()",          emptyUser.getName?.()); // undefined, no throw

    // Array access
    const arr = null;
    log("null?.[0]",                      arr?.[0]); // undefined, no throw

    // ── Nullish coalescing (??) ────────────────────────────────────────────────
    log("heading", "Nullish coalescing (??) vs OR (||)");

    // ?? triggers only for null/undefined
    log("null      ?? 'default'",         null      ?? "default");
    log("undefined ?? 'default'",         undefined ?? "default");
    log("0         ?? 'default'",         0         ?? "default"); // 0 — NOT replaced!
    log("''        ?? 'default'",         ""        ?? "default"); // "" — NOT replaced!
    log("false     ?? 'default'",         false     ?? "default"); // false — NOT replaced!

    log("heading", "⚠️ OR (||) replaces ALL falsy values — use ?? for optional values");
    log("0    || 3000", 0    || 3000); // 3000 ← WRONG if 0 is a valid port!
    log("0    ?? 3000", 0    ?? 3000); // 0    ← correct

    // ── Nullish assignment ─────────────────────────────────────────────────────
    log("heading", "Nullish assignment (??=)");

    const config = { theme: null, lang: "fr" };
    config.theme ??= "dark"; // assigned because theme is null
    config.lang  ??= "en";   // NOT assigned because "fr" is not null/undefined
    log("config after ??=", config);

    // ── Combining ?.  and ?? ────────────────────────────────────────────────
    log("heading", "Combining ?. and ??");

    const displayName = user.profile?.nickname ?? user.getName?.() ?? "Anonymous";
    log("profile?.nickname ?? getName?.() ?? 'Anonymous'", displayName);
}


// ─────────────────────────────────────────────────────────────────────────────
// § 17 — STORAGE
// ─────────────────────────────────────────────────────────────────────────────

const store = {
    get(key, fallback = null) {
        try {
            const item = localStorage.getItem(key);
            return item !== null ? JSON.parse(item) : fallback;
        } catch {
            return fallback;
        }
    },
    set(key, value) {
        try {
            localStorage.setItem(key, JSON.stringify(value));
            return true;
        } catch (err) {
            console.error("Storage write failed:", err);
            return false;
        }
    },
    remove(key) { localStorage.removeItem(key); },
};

function demoStorageWrite() {
    setPanel("out-storage");

    const data = {
        user:  { name: "Alice", role: "admin" },
        prefs: { theme: "dark", lang: "fr", fontSize: 16 },
        lastVisit: new Date().toISOString(),
    };

    const ok = store.set("codesamples_demo", data);
    log("store.set('codesamples_demo', data)", ok ? "✅ written" : "❌ failed");
    log("data written", data);
    log("(check Application tab in DevTools)", "Storage → Local Storage");
}

function demoStorageRead() {
    setPanel("out-storage");

    const data = store.get("codesamples_demo");
    if (data) {
        log("store.get('codesamples_demo')", data);
        log("data.user",        data.user);
        log("data.prefs.theme", data.prefs?.theme);
        log("data.lastVisit",   data.lastVisit);
    } else {
        log("Nothing in storage yet", "→ click 'Write to storage' first", "warn");
    }
}

function demoStorageClear() {
    store.remove("codesamples_demo");
    setPanel("out-storage");
    log("store.remove('codesamples_demo')", "✅ removed");
    log("store.get('codesamples_demo')", store.get("codesamples_demo")); // null
}


// ─────────────────────────────────────────────────────────────────────────────
// § 18 — SECURITY PITFALLS
// ─────────────────────────────────────────────────────────────────────────────

function demoSecurity() {
    setPanel("out-security");

    log("heading", "1. XSS via innerHTML — the most common JS vulnerability");

    const maliciousInput = '<img src="x" onerror="console.warn(\'XSS would fire here\')">';
    log("User input contains",             maliciousInput);
    log("element.textContent = input      ✅ safe — rendered as text, not HTML", "no XSS");

    // We demonstrate what textContent does (safe) vs what innerHTML would do (dangerous)
    // We do NOT actually set innerHTML with the malicious string here.
    const demoEl = document.createElement("div");
    demoEl.textContent = maliciousInput;
    log("textContent output (safe)",       demoEl.textContent);
    log("innerHTML with user input         ❌ DANGEROUS — never do this", "skipped intentionally");

    log("heading", "2. eval() — never use with user input");
    log("eval('2 + 2')         ✅ trivial example (no user input)", eval("2 + 2"));
    log("eval(userInput)       ❌ REMOTE CODE EXECUTION — never", "skipped intentionally");
    log("setTimeout(userInput) ❌ same risk — setTimeout accepts strings", "skipped intentionally");

    log("heading", "3. Prototype pollution");

    // Safe merge — blocks __proto__, constructor, prototype
    function safeMerge(target, source) {
        const blocked = new Set(["__proto__", "constructor", "prototype"]);
        for (const key of Object.keys(source)) {
            if (blocked.has(key)) {
                console.warn(`Blocked dangerous key: ${key}`);
                continue;
            }
            target[key] = source[key];
        }
        return target;
    }

    // Simulate a malicious payload (as if from JSON.parse of user input)
    const maliciousPayload = Object.create(null);
    Object.defineProperty(maliciousPayload, "__proto__", {
        value: { isAdmin: true },
        enumerable: true,
    });
    maliciousPayload.name = "safe value";

    const result = safeMerge({}, maliciousPayload);
    log("safeMerge({}, malicious payload)",  result);
    log("({}).isAdmin after safeMerge",       ({}).isAdmin); // still undefined ✅

    log("heading", "4. Sensitive data in storage");
    log("localStorage for auth tokens ❌",   "readable by any JS on the page (XSS pivot)");
    log("localStorage for passwords   ❌",   "never — ever");
    log("httpOnly cookies for tokens  ✅",   "JS cannot read httpOnly cookies");

    log("heading", "5. postMessage without origin check");
    log("Always verify event.origin before processing postMessage data", "✅");
}


// ─────────────────────────────────────────────────────────────────────────────
// § 19 — PERFORMANCE
// ─────────────────────────────────────────────────────────────────────────────

function setupPerformanceDemo() {
    // ── Debounce ──────────────────────────────────────────────────────────────
    function debounce(fn, delay) {
        let timer;
        return function(...args) {
            clearTimeout(timer);
            timer = setTimeout(() => fn.apply(this, args), delay);
        };
    }

    const debounceOutput = document.getElementById("debounce-output");
    const debounceInput  = document.getElementById("debounce-input");

    const handleDebounce = debounce((value) => {
        debounceOutput.textContent = `Fired! value = "${value}"`;
        debounceOutput.classList.add("fired");
        setTimeout(() => debounceOutput.classList.remove("fired"), 600);
    }, 400);

    debounceInput.addEventListener("input", (e) => {
        debounceOutput.textContent = `Waiting… (last key: "${e.key ?? "?"}") `;
        handleDebounce(e.target.value);
    });

    // ── Throttle ──────────────────────────────────────────────────────────────
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

    const scrollDisplay = document.getElementById("scroll-progress-display");

    const handleScroll = throttle(() => {
        const scrolled = window.scrollY;
        const total    = document.body.scrollHeight - window.innerHeight;
        const pct      = total > 0 ? Math.round((scrolled / total) * 100) : 0;
        scrollDisplay.textContent = `Scroll: ${pct}%  (Y: ${Math.round(scrolled)}px)`;
    }, 100); // at most once per 100ms

    window.addEventListener("scroll", handleScroll, { passive: true });

    // ── IntersectionObserver ──────────────────────────────────────────────────
    // Triggers when elements enter/leave the viewport — no scroll polling needed.
    const observer = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                entry.target.classList.add("visible");
                // Unobserve after first trigger (one-shot animation)
                observer.unobserve(entry.target);
            }
        });
    }, {
        threshold: 0.3, // 30% of the element must be visible
    });

    document.querySelectorAll(".fade-box").forEach(el => observer.observe(el));
}


// ─────────────────────────────────────────────────────────────────────────────
// NAVIGATION: Active section highlighting
// Uses IntersectionObserver to highlight the nav link for the visible section.
// This is the same IntersectionObserver pattern shown in § Performance.
// ─────────────────────────────────────────────────────────────────────────────

function setupNavHighlight() {
    const navLinks = document.querySelectorAll(".nav-list a");

    const sectionObserver = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                const id = entry.target.id;
                navLinks.forEach(link => {
                    link.classList.toggle("active", link.getAttribute("href") === `#${id}`);
                });
            }
        });
    }, {
        rootMargin: "-20% 0px -70% 0px", // trigger when section is in the upper 30% of viewport
    });

    document.querySelectorAll(".concept-section").forEach(section => {
        sectionObserver.observe(section);
    });
}


// ─────────────────────────────────────────────────────────────────────────────
// DEMO BUTTON WIRING
// Buttons with data-demo="<name>" call the matching handler.
// ─────────────────────────────────────────────────────────────────────────────

const demoHandlers = {
    "variables":        demoVariables,
    "types":            demoTypes,
    "functions":        demoFunctions,
    "objects":          demoObjects,
    "arrays":           demoArrays,
    "destructuring":    demoDestructuring,
    "spread-rest":      demoSpreadRest,
    "template-literals": demoTemplateLiterals,
    "classes":          demoClasses,
    "closures":         demoClosures,
    "promises":         demoPromises,
    "async-await":      demoAsyncAwait,
    "errors":           demoErrors,
    "optional-chaining": demoOptionalChaining,
    "storage-write":    demoStorageWrite,
    "storage-read":     demoStorageRead,
    "storage-clear":    demoStorageClear,
    "security":         demoSecurity,
};

document.addEventListener("click", (event) => {
    const btn = event.target.closest("[data-demo]");
    if (!btn) return;
    const handler = demoHandlers[btn.dataset.demo];
    if (handler) handler();
});


// ─────────────────────────────────────────────────────────────────────────────
// INIT
// ─────────────────────────────────────────────────────────────────────────────

setupDomDemo();
setupEventsDemo();
setupPerformanceDemo();
setupNavHighlight();

console.log(
    "%c CodeSamples — JavaScript Examples ",
    "background:#1e40af;color:#fff;font-weight:bold;padding:4px 8px;border-radius:4px"
);
console.log("All sections are interactive. Click 'Run demo' buttons to execute each concept.");
console.log("The code you see in <pre> blocks is the actual code running this page.");
