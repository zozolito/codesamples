# Java — CodeSamples

---

## 🇬🇧 English

### A language that was supposed to run on toasters

In 1991, James Gosling and a small team at Sun Microsystems were tasked with building software for **interactive cable TV set-top boxes**. The problem: every manufacturer used different hardware and a different operating system. Writing software for each one separately was madness.

Their solution was radical: instead of compiling code directly to machine instructions, compile it to an intermediate format — **bytecode** — and run that bytecode on a small virtual machine that could be ported anywhere. Write the software once. Run it on any device that has the virtual machine.

The project was called **Oak**. The target hardware turned out to be the wrong bet — interactive TV went nowhere — but the underlying idea was sound. In 1995, Sun rebranded it **Java** and pointed it at the exploding World Wide Web. It was a perfect match. Every computer had a different OS and processor, browsers could embed a tiny virtual machine, and suddenly Java applets ran in every browser on every machine.

The tagline: **"Write Once, Run Anywhere."**

---

### How the JVM works

When you write a Java program:

```
YourCode.java
      │
      │  javac (Java Compiler)
      ▼
YourCode.class  ← bytecode (not machine code — platform-neutral instructions)
      │
      │  JVM (Java Virtual Machine)
      ▼
Machine code   ← JVM translates bytecode to native instructions at runtime
```

The JVM is what makes Java portable. Every operating system (Windows, Linux, macOS, Android) runs its own JVM implementation — but they all understand the same bytecode.

Modern JVMs also include a **JIT compiler** (Just-In-Time): instead of interpreting bytecode line by line, the JVM compiles frequently-executed code paths into native machine code at runtime. This is why Java, despite the abstraction, can be very fast.

---

### Java's rise to dominance

| Year | Milestone |
|---|---|
| 1991 | James Gosling begins Project Oak at Sun Microsystems |
| 1995 | Renamed Java, released publicly — browser applets take the web by storm |
| 1996 | Java 1.0 — the beginning of the platform |
| 1998 | Java 2 (J2EE) — enterprise edition; Java becomes the default for large backend systems |
| 2004 | Java 5 — generics, autoboxing, enums, for-each loop — a major modernisation |
| 2006 | Sun open-sources Java under GPL |
| 2009 | Oracle acquires Sun Microsystems for $7.4 billion |
| 2014 | Java 8 — lambdas, streams, Optional — the most important Java release in a decade |
| 2017 | Java 9 — modules (Project Jigsaw); Java moves to 6-month release cadence |
| 2021 | Java 17 (LTS) — records, sealed classes, pattern matching — modern Java |
| 2023 | Java 21 (LTS) — virtual threads (Project Loom), sequenced collections |

**LTS versions** (Long-Term Support) receive security and bug fixes for years. For production: use Java 21 or Java 17. Java 8 is still widely deployed but increasingly legacy.

---

### Java today — where it runs

- **Enterprise backends** — Spring Boot powers a huge proportion of corporate Java applications (banking, insurance, logistics, government systems)
- **Big Data** — Apache Hadoop, Apache Kafka, Apache Spark are all JVM-based
- **Android** — the original Android language (more on this below)
- **Scientific computing** — Weka, Deeplearning4j
- **Embedded systems** — Java ME (Micro Edition) still runs on smart cards and feature phones

Java regularly ranks in the top 3 most-used programming languages in the world. Not because it's fashionable — because it runs the backbone of global infrastructure.

---

### Android — how Java ended up in a billion pockets

#### 2003: A startup with a strange idea

In October 2003, **Andy Rubin**, Rich Miner, Nick Sears, and Chris White founded **Android Inc.** in Palo Alto, California. The original pitch was an operating system for digital cameras — smarter cameras that could sync with computers. The market for that turned out to be too small. They pivoted to mobile phones.

The core idea: build a **free, open-source mobile OS** based on Linux, programmable in a widely-known language, that any hardware manufacturer could adopt and modify. No licensing fees. No dependency on a single company's SDK.

They chose Java as the programming language — it had a huge developer community, a mature tooling ecosystem, and the "write once, run anywhere" model was attractive for a platform targeting many different hardware configurations.

#### 2005: Google knocks on the door

In July 2005, Google acquired Android Inc. for approximately **$50 million**. The team of 22 employees stayed together. Andy Rubin kept running the project inside Google.

Why did Google want a mobile OS? Larry Page and Sergey Brin understood something important: if Windows had become the dominant platform of the PC era, and Microsoft had used that position to control access to users and steer them away from Google, the same thing could happen on mobile. Google needed to control the mobile platform — not to lock it down, but to ensure that no one else could lock them out.

#### 2007: Two announcements that changed everything

**January 9, 2007**: Steve Jobs walks on stage at Macworld and announces the iPhone. "An iPod, a phone, and an internet communicator." The mobile computing era begins in earnest.

**November 5, 2007**: Google announces the **Open Handset Alliance** and releases Android as open-source. 34 companies sign on: HTC, Samsung, Motorola, Qualcomm, Intel, T-Mobile, Sprint.

The Android team had been watching the iPhone announcement in silence. They had been working with a different form factor in mind — physical keyboard, scroll wheel, more BlackBerry than iPhone. They tore up a significant portion of their work and redesigned for touchscreens.

#### 2008: The first Android phone

**September 23, 2008**: The **HTC Dream** (sold as the T-Mobile G1 in the US) goes on sale. It runs Android 1.0. It has a physical QWERTY keyboard that slides out, a trackball for navigation, and a 3.2-inch touchscreen. The App Store for Android — the Android Market, later renamed Google Play — launches with 50 apps.

It wasn't as sleek as the iPhone. But it was open. Any developer could write an app. Any manufacturer could build a device. Any carrier could sell it.

#### The Dalvik VM — Java without the JVM

Here is where it gets technically interesting: Android could not simply ship Oracle's JVM. Licensing costs and performance on constrained mobile hardware were both problems.

Google engineer Dan Bornstein built **Dalvik**, a completely separate virtual machine designed for Android. Dalvik was not a JVM. It ran a different bytecode format (``.dex`` — Dalvik Executable). But critically: you could write your Android apps in Java, and a tool called ``dx`` would convert Java's ``.class`` bytecode to ``.dex`` bytecode.

The difference matters architecturally:
- Java's JVM is **stack-based** — instructions push and pop values from a stack
- Dalvik was **register-based** — more like actual CPU instructions, fewer operations needed

Register-based VMs generally execute faster and use less memory — important for phones with 256MB of RAM.

#### 2013: ART replaces Dalvik

Dalvik compiled bytecode **just-in-time** (JIT) — translating `.dex` instructions to native machine code at the moment they were needed. This works, but has overhead: every time you launch an app, some compilation happens.

In Android 4.4 (KitKat, 2013), Google introduced **ART** (Android Runtime) as an option, and made it the default in Android 5.0 (Lollipop, 2014). ART uses **ahead-of-time** (AOT) compilation: when you install an app, the entire thing is compiled to native code. Apps launch faster. Runtime performance is better. The tradeoff: installation takes a bit longer and the compiled code takes more storage.

Modern ART uses a hybrid approach: it profiles which code paths are hot and compiles those AOT, leaving rarely-used paths for JIT. The best of both worlds.

#### 2017: Kotlin enters the picture

At Google I/O 2017, Google announced **Kotlin** as a first-class language for Android. Kotlin is a JVM language developed by JetBrains (the company behind IntelliJ IDEA, the IDE that Android Studio is based on). It is fully interoperable with Java — Kotlin code compiles to the same JVM bytecode and can call Java libraries and vice versa.

By 2019, Google had announced Kotlin as the **preferred** language for Android development. Most new Android documentation, tutorials, and sample code is now written in Kotlin.

Java remains fully supported on Android. Billions of lines of Android code are Java. Any new Android developer should understand Java because:
1. The Android platform itself is written largely in Java
2. Most existing Android codebases they will encounter are Java
3. Understanding Java makes understanding Kotlin easier — Kotlin was designed to improve on Java, and you can only appreciate the improvements if you know what they're improving on

#### The Oracle lawsuit

In 2010, Oracle (which had acquired Sun Microsystems in 2009) sued Google for **$9 billion**, claiming that Android's use of 37 Java APIs — the method names, class structures, and package organisation — constituted copyright infringement.

Google's position: API declarations are not copyrightable — they are functional, like the key arrangement on a keyboard or the syntax of a formula. Copyright covers creative expression. An API is a specification, not a creative work.

The case wound through the courts for **eleven years**. In April 2021, the **US Supreme Court** ruled 6-2 in Google's favour: Google's use of the Java APIs was **fair use**. The decision was celebrated by developers worldwide — a ruling against Google would have made every API potentially copyrightable, fundamentally breaking how software is built.

---

### Why learn Java?

- **Ubiquity**: Java is on billions of devices. Understanding Java means being able to read and work with a massive proportion of existing software.
- **Concepts**: Java teaches OOP, generics, the type system, and software design patterns in a way that makes the same concepts in other languages easier to understand.
- **Android**: Even if you write Kotlin, you will encounter Java. Libraries are in Java. Documentation references Java. The platform is Java.
- **Enterprise**: Spring Boot, Jakarta EE, Hibernate, Maven, Gradle — if you work in a corporate environment, there is a substantial chance your backend runs on Java.
- **Maturity**: Java has 30 years of battle-tested libraries, tooling, and best practices. The ecosystem is unmatched.

---

### What's in this folder

| File / Folder | What it is |
|---|---|
| `Reference.md` | Comprehensive annotated Java reference — types to streams to security |
| `hello-world/` | A VS Code project demonstrating key Java idioms, annotated throughout |

---

## 🇫🇷 Français

### Un langage prévu pour les grille-pain

En 1991, James Gosling et une petite équipe chez Sun Microsystems devaient écrire des logiciels pour des **décodeurs de télévision interactive**. Le problème : chaque fabricant utilisait du matériel et un système d'exploitation différents. Écrire un logiciel séparé pour chacun était ingérable.

Leur solution : compiler non pas vers du code machine, mais vers un format intermédiaire — le **bytecode** — exécuté par une petite machine virtuelle portable. Écrire une fois. Exécuter partout.

Le projet s'appelait **Oak**. La télé interactive ne décolla jamais, mais l'idée était solide. En 1995, Sun le renomma **Java** et le dirigea vers le Web en pleine explosion. Le slogan : **"Write Once, Run Anywhere"**.

---

### Android — comment Java s'est retrouvé dans un milliard de poches

#### 2003 : Un projet de caméra intelligente

En octobre 2003, **Andy Rubin**, Rich Miner, Nick Sears et Chris White fondent **Android Inc.** à Palo Alto. L'idée de départ : un OS pour appareils photo numériques. Le marché est trop étroit. Ils pivotent vers les téléphones mobiles.

Le choix du Java pour le langage de programmation : une communauté énorme, des outils matures, et le modèle "écrire une fois, exécuter partout" idéal pour des dizaines de configurations matérielles différentes.

#### 2005 : Google rachète Android Inc.

En juillet 2005, Google acquiert Android Inc. pour environ **50 millions de dollars**. L'équipe de 22 personnes reste soudée. La logique de Google : si Windows avait dominé l'ère PC et utilisé cette domination pour éloigner les utilisateurs de Google, la même chose pouvait arriver sur mobile.

#### 2007 : Deux annonces qui changent tout

**9 janvier 2007** : Steve Jobs présente l'iPhone à Macworld.

**5 novembre 2007** : Google annonce l'Open Handset Alliance et publie Android en open source. 34 entreprises signent (HTC, Samsung, Motorola, Qualcomm…).

#### 2008 : Le premier téléphone Android

**23 septembre 2008** : Le **HTC Dream** (T-Mobile G1 aux États-Unis) est mis en vente. Il tourne sous Android 1.0. L'Android Market — renommé ensuite Google Play — lance avec 50 applications.

#### La Dalvik VM — Java sans la JVM d'Oracle

Android ne pouvait pas embarquer la JVM d'Oracle. L'ingénieur Dan Bornstein construit **Dalvik**, une machine virtuelle indépendante. Elle ne lit pas le bytecode Java standard (`.class`) mais un format `.dex` (Dalvik Executable). L'outil `dx` convertit automatiquement le bytecode Java en bytecode Dalvik.

Différence technique clé :
- La JVM Java est **à pile** (*stack-based*)
- Dalvik est **à registres** (*register-based*) — plus proche du fonctionnement réel du processeur, plus efficace en mémoire

#### 2014 : ART remplace Dalvik

**ART** (Android Runtime) remplace Dalvik dans Android 5.0. ART utilise la compilation **ahead-of-time** : l'application est compilée en code natif à l'installation, pas à l'exécution. Les apps démarrent plus vite. ART moderne combine AOT et JIT selon les chemins de code chauds.

#### 2017 : Kotlin entre en scène

À Google I/O 2017, Google annonce **Kotlin** comme langage de première classe pour Android. En 2019, Kotlin devient le langage **préféré**. Mais Java reste entièrement supporté — et le comprendre reste essentiel, car l'immense majorité du code Android existant est en Java.

#### Le procès Oracle

En 2010, Oracle attaque Google pour **9 milliards de dollars**, alléguant que l'utilisation des APIs Java dans Android constituait une violation du droit d'auteur. Onze ans plus tard, la **Cour Suprême des États-Unis** tranche en faveur de Google (2021) : l'utilisation des APIs était un *fair use*. Une décision capitale pour toute l'industrie logicielle.

---

### Jalons chronologiques

| Année | Événement |
|---|---|
| 1991 | James Gosling commence le Projet Oak chez Sun |
| 1995 | Renommage Java, première version publique |
| 2003 | Fondation d'Android Inc. par Andy Rubin et son équipe |
| 2004 | Java 5 — génériques, autoboxing, for-each |
| 2005 | Google rachète Android Inc. (~50M$) |
| 2007 | iPhone annoncé en janvier ; Android open-source annoncé en novembre |
| 2008 | Premier téléphone Android : HTC Dream (23 septembre) |
| 2009 | Oracle rachète Sun Microsystems (7,4 milliards $) |
| 2013 | ART introduit dans Android 4.4 |
| 2014 | Java 8 — lambdas, streams ; ART par défaut dans Android 5.0 |
| 2017 | Kotlin annoncé comme langage officiel Android à Google I/O |
| 2021 | Cour Suprême US : Google gagne le procès Oracle |
| 2023 | Java 21 (LTS) — threads virtuels, modern records |

---

### Ce qu'il y a dans ce dossier

| Fichier / Dossier | Description |
|---|---|
| `Reference.md` | Référence Java complète et annotée — des types aux streams |
| `hello-world/` | Projet VS Code démontrant les idiomes Java essentiels, entièrement commenté |
