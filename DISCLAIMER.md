# ⚠️ DISCLAIMER — Read this before using any code from this repository

> **Bilingual — English 🇬🇧 / Français 🇫🇷**

---

## 🇬🇧 English

### This is a learning resource. Not a production toolkit.

Let's be honest with each other from the start.

Every file in this repository was written with care, with pedagogical intent, and with a genuine desire to help you understand how things work under the hood. But care and good intent are not the same thing as *correctness for your specific context*. And no learning resource — no matter how thorough — should be blindly copy-pasted into production software.

This document exists to say that clearly, before you get burned.

---

### What these examples are

The code in this repository is **illustrative**. Each example is designed to demonstrate one concept at a time, as clearly as possible. That means:

- **Error handling is often simplified.** A real application handles far more edge cases than a tutorial can afford to show without becoming unreadable.
- **Security is discussed, but not exhaustively implemented.** A `snprintf` call is safer than a `sprintf` call — but it doesn't make your program *secure*. Security is a system-level property, not a one-line fix.
- **Performance is not the primary concern.** Some examples favour clarity over efficiency. That is intentional.
- **Platform assumptions are made.** An example that works on Linux with gcc -std=c11 may not compile identically on Windows with MSVC, or on an embedded ARM target with a custom toolchain.
- **The reference documentation may contain errors or omissions.** Languages evolve. Standards are updated. What was best practice in C99 may be superseded in C23. What is idiomatic C++17 may be discouraged in C++23. The maintainers of this repository are human.

---

### The copy-paste trap — real stories from the field

This is not a theoretical warning. Developers — experienced ones — have been seriously hurt by trusting code they didn't fully understand. Here are some of the stories worth knowing.

---

#### 📌 The Stack Overflow iOS security catastrophe (2019)

Researchers at the University of Toronto published a study analysing mobile apps that used a specific cryptographic code snippet widely upvoted on Stack Overflow. The answer showed how to generate a "secure" random number in iOS — except the implementation had a subtle but critical flaw: it seeded the random number generator with the current time in seconds, making the generated values *entirely predictable*.

**The result:** Hundreds of iOS applications — including some in the medical and financial sectors — shipped with a broken random number generator, directly copied from a 10-year-old Stack Overflow answer with thousands of upvotes and no warning.

The developers who used it were not careless. They were *trusting*. They assumed that a highly-voted answer had been peer-reviewed. It had not.

> **Lesson:** Upvotes measure popularity, not correctness. A cryptographic function that *looks* right and *compiles* successfully can still be fundamentally broken.

---

#### 📌 GitHub Copilot generates vulnerable code — 40% of the time (2021, NYU study)

A research team at NYU trained a large language model (similar to GitHub Copilot) on public code and then tested how often it produced *security-relevant* code with vulnerabilities. In security-sensitive scenarios — buffer handling, authentication, cryptography — the model produced vulnerable output approximately **40% of the time**.

The vulnerabilities included classic issues: buffer overflows, use-after-free, SQL injection, hardcoded credentials, improper input validation.

The code *compiled*. It *ran*. It *looked plausible*. It was wrong.

A follow-up study in 2023 showed that developers who used AI coding assistants were *more likely* to introduce security vulnerabilities than those who didn't, partly because AI-generated code created a false sense of confidence that reduced careful review.

> **Lesson:** An AI assistant writes code that *could* be correct. It does not know your security model, your threat landscape, your data sensitivity, or your deployment context. It is autocomplete for code, not an architect.

---

#### 📌 The tutorial that taught a generation of PHP developers to write SQL injections

In the late 2000s and early 2010s, the most popular PHP tutorials on the internet demonstrated database queries like this:

```php
$result = mysql_query("SELECT * FROM users WHERE name='" . $_GET['name'] . "'");
```

Millions of websites were built this way. The tutorials were well-intentioned and technically functional — the code ran. But it was vulnerable to SQL injection: an attacker could set `name` to `' OR '1'='1` and bypass authentication entirely.

This pattern, copy-pasted from tutorial to tutorial, forum post to forum post, contributed to SQL injection remaining the **#1 web application vulnerability** for over a decade — and it still appears today in legacy systems.

The developers who wrote this code were not incompetent. They were following the examples they were given, without understanding *why* those examples were dangerous.

> **Lesson:** A tutorial that works is not a tutorial that is safe. Understanding *why* code is written a certain way is the only protection against using it wrongly.

---

#### 📌 Samsung leaks proprietary source code through ChatGPT (2023)

In April 2023, Samsung engineers — trying to be productive — pasted proprietary source code, internal meeting notes, and hardware specifications into ChatGPT to help them debug and summarise. Three separate incidents occurred within 20 days.

That data became part of OpenAI's training pipeline. Samsung's trade secrets — the internal workings of their semiconductor business — left the building, forever, because developers trusted a tool without understanding its data policy.

> **Lesson:** AI tools are not confidential. The code and data you paste into them may not stay yours. Never paste proprietary code, credentials, customer data, or trade secrets into any AI assistant.

---

#### 📌 The developer who deployed "working" AI-generated authentication

This one is composite — it describes a pattern that has been reported hundreds of times in developer forums since 2023, anonymised here.

A junior developer, under deadline pressure, asked an AI assistant to write a user authentication function. The function was generated in seconds, looked professional, handled edge cases in the code comments, and passed basic testing. It was deployed to production.

What the developer did not notice:
- The password comparison used a non-constant-time string comparison — making it vulnerable to timing attacks.
- The session token was generated with `rand()` seeded with `time()` — predictable.
- The error messages distinguished between "user not found" and "wrong password" — leaking username validity.
- There was no rate limiting on the login endpoint — enabling brute force.

None of these issues caused a compilation error. None caused a test failure. All of them are textbook authentication vulnerabilities that an experienced security reviewer would catch in minutes.

The developer trusted the code because it *worked*. "Working" and "correct" are not the same thing.

> **Lesson:** Code generated by AI — or copied from any source — must be *understood*, not just deployed. If you cannot explain every line of your authentication, cryptography, or data handling code, you should not ship it.

---

### The deeper truth about mastery

There is a difference between *using* code and *understanding* code.

Using code means: the function runs, the output looks right, the tests pass, the deadline is met.

Understanding code means: you know *why* each line is there, what happens if the input is unexpected, what happens under memory pressure, what the security implications are, what the edge cases are, and what you would change if the requirements shifted.

The gap between these two states is where bugs live. It is where security vulnerabilities incubate. It is where production incidents are born.

Copy-paste coding does not close that gap. It widens it. Each piece of code you use without understanding is a piece of your system whose behaviour you cannot predict.

**Your code is your responsibility.** Not the tutorial's. Not the AI's. Not ours. Yours.

If something goes wrong in production — and eventually something will — the question you will face is: "Do you understand why this happened?" If the answer is "I'm not sure, I copied that part," you will have nowhere to stand.

---

### What to do instead

None of this means you should never consult external resources. It means you should consult them *actively*, not passively.

**When you find code that does what you need:**

1. **Read it completely.** Not skim. Read every line. If you don't understand a line, look it up.
2. **Ask what it assumes.** Does it assume UTF-8 input? Does it assume a 64-bit platform? Does it assume the caller already validated the input?
3. **Ask what happens when it fails.** What if `malloc` returns `NULL`? What if the file doesn't exist? What if the network drops mid-request?
4. **Ask what it doesn't do.** A function that opens a file correctly may not close it on error. A function that validates input may not sanitise it.
5. **Rewrite it.** Seriously. Type it out yourself, in your own style, in your own codebase. The act of rewriting forces understanding that reading does not. When you retype code, you engage with it — you make choices, you notice things, you own it.
6. **Test the edge cases.** Empty input. Null pointers. Maximum values. Minimum values. Concurrent access. Out-of-memory conditions.

---

### A note on this repository specifically

The code and documentation in this repository may contain:
- Simplified implementations that omit production-grade error handling
- Platform-specific behaviour that differs on your target system
- Outdated patterns as language standards evolve
- Inaccuracies in the explanations — we are not infallible

If you find an error, open an issue or a pull request. That is how this resource improves.

If you find something you don't understand, *that is the most important thing in this repository*. That uncertainty is a signal. Follow it. Read the standard. Read the specification. Build a test. Ask a question. Do not paper over the uncertainty by deploying anyway.

---

### One last thing

The reason this repository exists is precisely because we believe that understanding matters — that the world is made worse when developers ship code they don't understand, and better when they take the time to learn.

You are reading this disclaimer. That already puts you ahead of most people who copy code from the internet.

Keep going.

---

---

## 🇫🇷 Français

### C'est une ressource pédagogique. Pas une boîte à outils pour la production.

Soyons honnêtes dès le départ.

Chaque fichier de ce dépôt a été écrit avec soin, avec une intention pédagogique sincère et un vrai désir de t'aider à comprendre comment les choses fonctionnent sous le capot. Mais le soin et la bonne volonté ne sont pas la même chose que *l'exactitude pour ton contexte spécifique*. Et aucune ressource d'apprentissage — aussi complète soit-elle — ne devrait être copiée-collée aveuglément dans un logiciel de production.

Ce document existe pour le dire clairement, avant que tu ne t'y brûles.

---

### Ce que sont ces exemples

Le code de ce dépôt est **illustratif**. Chaque exemple est conçu pour démontrer un concept à la fois, aussi clairement que possible. Cela signifie :

- **La gestion des erreurs est souvent simplifiée.** Une vraie application gère bien plus de cas limites qu'un tutoriel peut se permettre de montrer sans devenir illisible.
- **La sécurité est abordée, mais pas exhaustivement implémentée.** Un appel à `snprintf` est plus sûr que `sprintf` — mais il ne rend pas ton programme *sécurisé*. La sécurité est une propriété du système dans son ensemble, pas un correctif sur une seule ligne.
- **La performance n'est pas la priorité.** Certains exemples privilégient la clarté à l'efficacité. C'est intentionnel.
- **Des hypothèses de plateforme sont faites.** Un exemple qui fonctionne sous Linux avec gcc -std=c11 peut ne pas compiler de la même façon sous Windows avec MSVC, ou sur une cible ARM embarquée avec une chaîne d'outils personnalisée.
- **La documentation de référence peut contenir des erreurs ou des omissions.** Les langages évoluent. Les standards sont mis à jour. Ce qui était une bonne pratique en C99 peut être dépassé en C23. Les mainteneurs de ce dépôt sont humains.

---

### Le piège du copier-coller — des histoires vraies du terrain

Avertissement pas théorique. Des développeurs — expérimentés — se sont gravement fait piéger en faisant confiance à du code qu'ils ne comprenaient pas vraiment. Voici quelques histoires à connaître.

---

#### 📌 La catastrophe de sécurité iOS venue de Stack Overflow (2019)

Des chercheurs de l'Université de Toronto ont analysé des applications mobiles utilisant un extrait de code cryptographique très bien noté sur Stack Overflow. La réponse montrait comment générer un nombre aléatoire "sécurisé" sous iOS — sauf que l'implémentation avait un défaut subtil mais critique : elle initialisait le générateur aléatoire avec l'heure courante en secondes, rendant les valeurs générées *entièrement prévisibles*.

**Résultat :** Des centaines d'applications iOS — dont certaines dans les secteurs médical et financier — ont été livrées avec un générateur aléatoire cassé, copié directement depuis une réponse Stack Overflow vieille de 10 ans avec des milliers de votes positifs et aucun avertissement.

Les développeurs qui l'ont utilisée n'étaient pas négligents. Ils *faisaient confiance*. Ils pensaient qu'une réponse très votée avait été relue par des pairs. Elle ne l'avait pas été.

> **Leçon :** Les votes mesurent la popularité, pas la justesse. Une fonction cryptographique qui *semble* correcte et *compile* avec succès peut néanmoins être fondamentalement cassée.

---

#### 📌 GitHub Copilot génère du code vulnérable — 40 % du temps (étude NYU, 2021)

Une équipe de recherche de NYU a testé la fréquence à laquelle un modèle de langage génère du code *vulnérable* dans des contextes sensibles à la sécurité. En situations impliquant la gestion de tampons, l'authentification ou la cryptographie, le modèle produisait du code vulnérable environ **40 % du temps**.

Les vulnérabilités incluaient des classiques : dépassements de tampon, accès après libération, injection SQL, identifiants codés en dur, validation d'entrée défaillante.

Le code *compilait*. Il *tournait*. Il *semblait plausible*. Il était erroné.

Une étude de suivi en 2023 a montré que les développeurs utilisant des assistants IA étaient *plus susceptibles* d'introduire des vulnérabilités de sécurité que ceux qui n'en utilisaient pas, en partie parce que le code généré par IA crée un faux sentiment de confiance qui réduit la relecture attentive.

> **Leçon :** Un assistant IA écrit du code qui *pourrait* être correct. Il ne connaît pas ton modèle de sécurité, ta surface d'attaque, la sensibilité de tes données ou ton contexte de déploiement. C'est de l'autocomplétion pour le code, pas un architecte.

---

#### 📌 Le tutoriel qui a appris à toute une génération de développeurs PHP à écrire des injections SQL

À la fin des années 2000, les tutoriels PHP les plus populaires sur internet montraient des requêtes de base de données comme ceci :

```php
$result = mysql_query("SELECT * FROM users WHERE name='" . $_GET['name'] . "'");
```

Des millions de sites web ont été construits ainsi. Les tutoriels étaient bien intentionnés et techniquement fonctionnels — le code tournait. Mais il était vulnérable aux injections SQL : un attaquant pouvait définir `name` à `' OR '1'='1` et contourner l'authentification complètement.

Ce schéma, copié de tutoriel en tutoriel, de forum en forum, a contribué à faire de l'injection SQL la **vulnérabilité web n°1** pendant plus d'une décennie — et elle apparaît encore aujourd'hui dans des systèmes legacy.

Les développeurs qui ont écrit ce code n'étaient pas incompétents. Ils suivaient les exemples qu'on leur donnait, sans comprendre *pourquoi* ces exemples étaient dangereux.

> **Leçon :** Un tutoriel qui fonctionne n'est pas un tutoriel qui est sûr. Comprendre *pourquoi* le code est écrit d'une certaine façon est la seule protection contre son utilisation erronée.

---

#### 📌 Samsung fait fuiter son code source propriétaire via ChatGPT (2023)

En avril 2023, des ingénieurs de Samsung — cherchant à être productifs — ont collé du code source propriétaire, des notes de réunions internes et des spécifications matérielles dans ChatGPT pour les aider à déboguer et à résumer. Trois incidents distincts se sont produits en moins de 20 jours.

Ces données sont devenues une partie du pipeline d'entraînement d'OpenAI. Les secrets commerciaux de Samsung — le fonctionnement interne de leur activité de semi-conducteurs — ont quitté l'entreprise, pour toujours, parce que des développeurs ont fait confiance à un outil sans comprendre sa politique de données.

> **Leçon :** Les outils IA ne sont pas confidentiels. Le code et les données que tu y colles peuvent ne pas rester les tiens. Ne colle jamais du code propriétaire, des identifiants, des données clients ou des secrets commerciaux dans un assistant IA quelconque.

---

#### 📌 Le développeur qui a déployé une authentification "fonctionnelle" générée par IA

Celui-ci est composite — il décrit un schéma rapporté des centaines de fois dans les forums de développeurs depuis 2023, anonymisé ici.

Un développeur junior, sous pression de délai, a demandé à un assistant IA d'écrire une fonction d'authentification utilisateur. La fonction a été générée en quelques secondes, avait l'air professionnelle, gérait des cas limites dans les commentaires, et a passé les tests de base. Elle a été déployée en production.

Ce que le développeur n'a pas remarqué :
- La comparaison de mot de passe utilisait une comparaison de chaînes non-à-durée-constante — vulnérable aux attaques temporelles.
- Le jeton de session était généré avec `rand()` initialisé par `time()` — prévisible.
- Les messages d'erreur distinguaient "utilisateur introuvable" de "mot de passe erroné" — révélant la validité des noms d'utilisateur.
- Il n'y avait aucune limitation de débit sur l'endpoint de connexion — permettant la force brute.

Aucun de ces problèmes ne cause une erreur de compilation. Aucun ne fait échouer un test. Tous sont des vulnérabilités d'authentification classiques qu'un relecteur de sécurité expérimenté remarquerait en quelques minutes.

Le développeur faisait confiance au code parce qu'il *fonctionnait*. "Fonctionner" et "être correct" ne sont pas la même chose.

> **Leçon :** Le code généré par IA — ou copié depuis n'importe quelle source — doit être *compris*, pas seulement déployé. Si tu ne peux pas expliquer chaque ligne de ton code d'authentification, de cryptographie ou de gestion de données, tu ne devrais pas le mettre en production.

---

### La vérité profonde sur la maîtrise

Il y a une différence entre *utiliser* du code et *comprendre* du code.

Utiliser du code signifie : la fonction tourne, la sortie semble correcte, les tests passent, le délai est respecté.

Comprendre du code signifie : tu sais *pourquoi* chaque ligne est là, ce qui se passe si l'entrée est inattendue, ce qui se passe sous pression mémoire, quelles sont les implications de sécurité, quels sont les cas limites, et ce que tu changerais si les exigences évoluaient.

L'écart entre ces deux états, c'est là que vivent les bugs. C'est là que s'incubent les vulnérabilités de sécurité. C'est là que naissent les incidents de production.

Le code copié-collé ne comble pas cet écart. Il l'élargit. Chaque morceau de code que tu utilises sans comprendre est un morceau de ton système dont tu ne peux pas prédire le comportement.

**Ton code est ta responsabilité.** Pas celle du tutoriel. Pas celle de l'IA. Pas la nôtre. La tienne.

Si quelque chose tourne mal en production — et tôt ou tard quelque chose tournera mal — la question à laquelle tu feras face est : "Comprends-tu pourquoi c'est arrivé ?" Si la réponse est "Je ne suis pas sûr, j'ai copié cette partie", tu n'auras nulle part où te tenir.

---

### Que faire à la place

Rien de tout cela ne signifie que tu ne devrais jamais consulter de ressources externes. Cela signifie que tu devrais les consulter *activement*, pas passivement.

**Quand tu trouves du code qui fait ce dont tu as besoin :**

1. **Lis-le entièrement.** Pas en diagonale. Chaque ligne. Si tu ne comprends pas une ligne, cherche.
2. **Demande-toi ce qu'il assume.** Est-ce qu'il suppose une entrée en UTF-8 ? Une plateforme 64 bits ? Que l'appelant a déjà validé l'entrée ?
3. **Demande-toi ce qui se passe quand il échoue.** Et si `malloc` retourne `NULL` ? Et si le fichier n'existe pas ? Et si le réseau tombe en plein milieu d'une requête ?
4. **Demande-toi ce qu'il ne fait pas.** Une fonction qui ouvre un fichier correctement peut ne pas le fermer en cas d'erreur. Une fonction qui valide l'entrée peut ne pas la nettoyer.
5. **Réécris-le.** Sérieusement. Tape-le toi-même, dans ton style, dans ta base de code. L'acte de réécriture force une compréhension que la simple lecture ne donne pas. Quand tu retapes du code, tu t'y engages — tu fais des choix, tu remarques des choses, tu te l'appropries.
6. **Teste les cas limites.** Entrée vide. Pointeurs nuls. Valeurs maximales. Valeurs minimales. Accès concurrent. Conditions de mémoire insuffisante.

---

### Une note sur ce dépôt en particulier

Le code et la documentation de ce dépôt peuvent contenir :
- Des implémentations simplifiées qui omettent la gestion d'erreurs de niveau production
- Des comportements spécifiques à une plateforme qui diffèrent sur ton système cible
- Des patterns dépassés au fil de l'évolution des standards de langages
- Des inexactitudes dans les explications — nous ne sommes pas infaillibles

Si tu trouves une erreur, ouvre une issue ou une pull request. C'est ainsi que cette ressource s'améliore.

Si tu trouves quelque chose que tu ne comprends pas, *c'est la chose la plus importante de ce dépôt*. Cette incertitude est un signal. Suis-le. Lis le standard. Lis la spécification. Construis un test. Pose une question. Ne dissimule pas l'incertitude en déployant quand même.

---

### Une dernière chose

Si ce dépôt existe, c'est précisément parce que nous croyons que la compréhension est ce qui compte — que le monde se dégrade quand les développeurs livrent du code qu'ils ne comprennent pas, et qu'il s'améliore quand ils prennent le temps d'apprendre.

Tu lis ce disclaimer. Ça te place déjà devant la plupart des gens qui copient du code sur internet.

Continue.

---

*Ce fichier fait partie du projet CodeSamples — une ressource d'apprentissage gratuite et communautaire.*
*This file is part of the CodeSamples project — a free, community learning resource.*
