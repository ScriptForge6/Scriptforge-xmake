<center>
 
 <img src="src/ScriptForge.png" alt="Scriptforge" width="100"/>
 
 <h1>Scriptforge</h1>
 
 <h2>An open-source library with many strange and wonderful features</h2>

**Other Language:** [Chinese](README-CN.md)

[Changelog](CHANGELOG.md)

</center>

# ⚠️ Maintenance Notice

The library is currently undergoing a major refactoring and update. 
Please note that:

- **Compilation errors** may occur in the current version
- **API stability** is not guaranteed during this period  
- **Some features** may be temporarily unavailable

We recommend using the latest stable release until the update is complete.


---

## Important Notice

This project is programmed against the `C++23` standard. Due to inconsistent compiler‑side support for `C++23` features, this library is not usable with certain compilers. Primary compatibility and daily development target **MSVC**. Support for GCC and Clang will be added incrementally alongside compiler and project updates, and is **not guaranteed**.

---

## Quick Start

- [**AntiDebug**](#antidebug)
  - [Basic Usage](#basic-usage)
  - [Custom Anti‑Debug Detection & Self‑Termination Logic](#custom%E2%80%91anti%E2%80%91debug%E2%80%91detection--self%E2%80%91termination-logic)
- [**Argv**](#argv)
- [**Err**](#err)
  - [Using `Error` Directly](#using-error-directly)
  - [Using `BasicError`](#using-basicerror)
- [**LanguageCode**](#languagecode)
- [**Local**](#local)
  - [Language Enum](#language-enum)
  - [LANG_NAME_TO_ENUM](#lang_name_to_enum)
  - [ISO639_1_TO_ENUM](#iso639_1_to_enum)
  - [ENUM_TO_LANG_NAME](#enum_to_lang_name)
  - [ENUM_TO_ISO639_1](#enum_to_iso639_1)
- [**Msg**](#msg)
  - [Using `Message` Directly](#using-message-directly)
  - [InformationLevel](#informationlevel)
- [**Pch**](#pch)

### AntiDebug

Type alias definition for `AntiDebug`:

```
using Scriptforge::AntiDebug::AntiDebugger = ADNS::ADCL;
```

> 
> `ADNS` and `ADCL` are obfuscation macros.

#### Basic Usage

```
import Scriptforge.AntiDebug;

int main() {
    Scriptforge::AntiDebug::AntiDebugger debugger;
    debugger.start();

    //TODO

    debugger.stop();

    return 0;
}
```

> 
> - The destructor automatically invokes `stop()`.
> - Calling `start()` blocks the main thread for **100 ms**.

#### Custom Anti‑Debug Detection & Self‑Termination Logic

```
import Scriptforge.AntiDebug;
import Scriptforge.AntiDebug.RandomDefine;   // Expands obfuscation macros

class AntiDebugger : public Scriptforge::AntiDebug::AntiDebugger {
private:

    // Anti‑debug detection routine
    bool F4() noexcept override {

	      //TODO

        return is_attached;
    }

    // Self‑termination routine
    void F5() noexcept override {
			
        //TODO

    }
};

int main() {
    AntiDebugger debugger;
    debugger.start();

    //TODO

    debugger.stop();
    return 0;
}
```

> 
> - `F4` and `F5` are obfuscation macros.
> - The destructor automatically invokes `stop()`.
> - Calling `start()` blocks the main thread for **100 ms**.
> - You **must import** `Scriptforge.AntiDebug.RandomDefine` to expand the function‑name macros.

---

### Argv

> 
> **Pending refactor. Refer to source code for ground‑truth behavior.**

Type alias definition for `Argv`:

```
template<Scriptforge::Argv::isHash HashT = Scriptforge::Argv::Hash::FNV_1a_32>
    class Scriptforge::Argv::ArgvCli;
```

Example:

```
import <cstdint>;
import <iostream>;
import <vector>;
import <string_view>;
import Scriptforge.Argv;

struct ArgvCommandA {
    static constexpr std::string_view name = "--a";
    static constexpr std::string_view shortName = "-a";
    static void run(
        std::vector<std::string>::iterator it,
        std::string_view arg,
        std::ostream& os,
        std::ostream& err,
        std::istream& is
    ) {
        os << "Running command: " << arg << std::endl;
    }
};

struct ArgvCommandB {
    static constexpr std::string_view name = "--b";
    static constexpr std::string_view shortName = "-b";
    static void run(
        std::vector<std::string>::iterator it, // Points to current command; used to read subsequent arguments
        std::string_view arg,                  // The actual command‑name invoked
        std::ostream& os,                      // Output stream
        std::ostream& err,                     // Error output stream
        std::istream& is                       // Input stream
    ) {
        os << "Running command: " << arg << std::endl;
    }
};

struct FNV_1a_32 {
    static constexpr uint32_t hash(std::string_view s) {
    uint32_t h = 0x811C9DC5u;
    for (char c : s)
        h = (h ^ (uint8_t)c) * 0x01000193u;
    return h;
    }
};

int main(int argc, char* argv[]) {
    Scriptforge::Argv::ArgvCli<FNV_1a_32> cli(
        argc,
        argv,
        std::cout,
        std::cerr,
        std::cin
    );

    cli.run<ArgvCommandA, ArgvCommandB>();
    return 0;
}
```

> 
> - The three streams enable input/output redirection.
> - `FNV_1a_32` is built‑in; you may instantiate `Scriptforge::Argv::ArgvCli<>` directly with it.
> - Initialization via the `init()` function is also supported.

---

### Err

#### Using `Error` Directly

Type alias definition for `Error`:

```
using Scriptforge::Err::Error = Scriptforge::Err::BasicError<>;
```

```
import Scriptforge.Err;

int main() {
    try{
        throw Scriptforge::Err::Error{
            "Code",
            "Message",
            Scriptforge::Msg::InformationLevel::Error
            };
    }
    catch(const Scriptforge::Err::Error& e) {
        std::cerr << e << std::endl;        // Output: [YYYY‑MM‑DD HH:MM:SS] [Error] [Code] Message
        std::cerr << e.code() << std::endl; // Output: Code
        std::cerr << e.what() << std::endl; // Output: Message
    }
}
```

> 
> - See [`Msg`](#informationlevel) for the definition of `Scriptforge::Msg::InformationLevel`.
> - Additional member functions are documented under [`Msg`](#msg) (`Error` inherits from `Msg`).

#### Using `BasicError`

Definition of `BasicError`:

```
template<typename CodeT = std::string, typename T = std::string, typename Clock = std::chrono::system_clock>
    requires Scriptforge::Err::ErrorRequires<CodeT, T, Clock>
class Scriptforge::Err::BasicError : public Scriptforge::Msg::BasicMessage<T, Clock>;
```

```
import <chrono>;
import <string>;
import Scriptforge.Err;

enum class Error {
    Error1,
    Error2
};

int main() {
    try{
        throw Scriptforge::Err::BasicError<
        Error,
        std::u8string,
        std::chrono::utc_clock
        >{
            Error::Error1,
            "Message",
            Scriptforge::Msg::InformationLevel::Error,
            std::chrono::utc_clock::now()
            };
    }
    catch(const Scriptforge::Err::Error& e) {
        std::cerr << e << std::endl;        // Case 1: Compile‑time error: no stream‑operator overload for enum `Error`
                                            // Case 2: Output [YYYY‑MM‑DD HH:MM:SS] [Error] [0] Message
                                            // (implicit conversion of enum to integer)
        e.code();                           // Returns: Error::Error1
        std::cerr << e.what() << std::endl; // Returns: Message
    }
}
```

---

### LanguageCode

> 
> Primarily intended for use together with [`Local`](#local).

#### Language Enum

Definition of the `Language` enum:

```
enum class Language : int {
    Neutral = 0x00,
    Invariant = 0x7f,
    Afrikaans = 0x36,
    Albanian = 0x1c,
    Amharic = 0x5e,
    Arabic = 0x01,
    Armenian = 0x2b,
    Assamese = 0x4d,
    Azerbaijani = 0x2c,
    ...
};
```

> 
> Full definition: [`Scriptforge.LanguageCode.ixx`](scriptforge_lib/Scriptforge.LanguageCode.ixx).

#### LANG_NAME_TO_ENUM

> 
> Maintained for developers targeting Win32 API for quick onboarding.

Definition of `LANG_NAME_TO_ENUM`:

```
inline const std::map<std::string, Language> LANG_NAME_TO_ENUM = {
    {"LANG_NEUTRAL", Language::Neutral},
    {"LANG_INVARIANT", Language::Invariant},
    {"LANG_AFRIKAANS", Language::Afrikaans},
    {"LANG_ALBANIAN", Language::Albanian},
    {"LANG_AMHARIC", Language::Amharic},
    {"LANG_ARABIC", Language::Arabic},
    {"LANG_ARMENIAN", Language::Armenian},
    {"LANG_ASSAMESE", Language::Assamese},
    {"LANG_AZERBAIJANI", Language::Azerbaijani},
    ...
};
```

> 
> Full definition: [`Scriptforge.LanguageCode.ixx`](scriptforge_lib/Scriptforge.LanguageCode.ixx).

#### ISO639_1_TO_ENUM

Definition of `ISO639_1_TO_ENUM`:

```
inline const std::map<std::string, Language> ISO639_1_TO_ENUM = {
    {"af", Language::Afrikaans},
    {"sq", Language::Albanian},
    {"am", Language::Amharic},
    {"ar", Language::Arabic},
    {"hy", Language::Armenian},
    {"as", Language::Assamese},
    {"az", Language::Azerbaijani},
    ...
};
```

> 
> Full definition: [`Scriptforge.LanguageCode.ixx`](scriptforge_lib/Scriptforge.LanguageCode.ixx).

#### ENUM_TO_LANG_NAME

Definition of `ENUM_TO_LANG_NAME`:

```
inline const std::map<Language, std::string> ENUM_TO_LANG_NAME = []() {
    std::map<Language, std::string> m;
    for (const auto& pair : LANG_NAME_TO_ENUM) {
        m[pair.second] = pair.first;
    }
    return m;
}();
```

#### ENUM_TO_ISO639_1

Definition of `ENUM_TO_ISO639_1`:

```
inline const std::map<Language, std::string>& ENUM_TO_ISO639_1() {
    static const std::map<Language, std::string> ENUM_TO_ISO639_1 = {
        { Language::Afrikaans, "af" },
        { Language::Albanian, "sq" },
        { Language::Amharic, "am" },
        { Language::Arabic, "ar" },
        { Language::Armenian, "hy" },
        { Language::Assamese, "as" },
        { Language::Azerbaijani, "az" },
        ...
    };
    return ENUM_TO_ISO639_1;
}
```

> 
> Full definition: [`Scriptforge.LanguageCode.ixx`](scriptforge_lib/Scriptforge.LanguageCode.ixx).

---

### Local

> 
> **Pending refactor. Refer to source code for ground‑truth behavior.**

#### Lang

Definition of the `Lang` class:

> 
> Usage overview: place JSON translation files named after ISO 639‑1 language codes inside your language directory.

```
class Lang {
public:
    Lang(Scriptforge::LanguageCode::Language lang = Scriptforge::LanguageCode::Language::Neutral, fs::path p = { "./lang" });
    void reload();
    void setLocale(const Scriptforge::LanguageCode::Language lang);
    Scriptforge::LanguageCode::Language getLanguageCode() const;
    template<typename T>
        requires is_basic_string<T>
    T getLanguageNameISO639_1() const;
    template<typename T>
        requires is_basic_string<T>
    T getLanguageName() const;
    fs::path getLangPath() const;
    void setLangPath(const fs::path& path);
    template<typename T>
        requires is_basic_string<T>
    T at(const T& key) const;
    template<typename T>
        requires is_basic_string<T>
    const JsonWithFormat atJ(const T& key) const;
    template<typename T>
        requires is_basic_string<T>
    T value(const T& key, const T& defaultValue) const;
    template<typename T, typename... Args>
        requires is_basic_string<T>
    T format(const T& key, Args&&... args) const;
    template<typename T>
        requires is_basic_string<T>
    bool has(const T& key) const;
    template<typename T>
        requires is_basic_string<T>
    std::vector<T> getKeys() const;
    const JsonWithFormat& getJson() const;
    bool isLoaded() const;
    template<typename T>
        requires is_basic_string<T>
    JsonWithFormat operator[](const T& key);
    private:
    void loadLanguageFile(Scriptforge::LanguageCode::Language lang, fs::path path);
    void LanguageIsLegal(Scriptforge::LanguageCode::Language lang) const;
    void pathIsLegal(const fs::path& path) const;
    Scriptforge::LanguageCode::Language m_lang;
    JsonWithFormat j;
    fs::path m_path;
};
```

---

### Msg

#### Using `Message` Directly

Type alias definition for `Message`:

```
using Scriptforge::Msg::Message = Scriptforge::Msg::BasicMessage<>;
```

Example:

```
import <iostream>;
import Scriptforge.Msg;

int main() {
    Scriptforge::Msg::Message msg{
        "Message",
        Scriptforge::Msg::InformationLevel::Warning
    };

    std::cout << msg << std::endl;           // Output: [YYYY‑MM‑DD HH:MM:SS] [Warning] Message
    msg.level();                             // Returns: Scriptforge::Msg::InformationLevel::Warning
    std::cout << msg.message() << std::endl; // Output: Message
    msg.time();                              // Returns `std::chrono::system_clock::time_point` of message creation

    return 0;
}
```

#### InformationLevel

---

### Pch

Precompiled header. Use via `import "Scriptforge.Pch.hpp";`.

---

## Project File Tree

> 
> Auto‑generated by GitHub Actions

<!-- TREE_START -->
```
Scriptforge-xmake
├── CHANGELOG-CN.md
├── CHANGELOG.md
├── LICENSE-THIRD-PARTY.txt
├── LICENSE.txt
├── README-CN.md
├── README.md
├── assets
│   └── ScriptForge.png
├── define.lua
├── pre
│   ├── main.cpp
│   └── xmake.lua
├── repo_tree.md
├── scriptforge_lib
│   ├── Scriptforge.AntiDebug.ixx
│   ├── Scriptforge.Argv.ixx
│   ├── Scriptforge.Base64.ixx
│   ├── Scriptforge.ByteBuffer.ixx
│   ├── Scriptforge.Concept.ixx
│   ├── Scriptforge.Err.ixx
│   ├── Scriptforge.ErrCode.ixx
│   ├── Scriptforge.LanguageCode.ixx
│   ├── Scriptforge.Local.ixx
│   ├── Scriptforge.Log.ixx
│   ├── Scriptforge.Msg.ixx
│   ├── Scriptforge.Pch.hpp
│   ├── Scriptforge.RingBuffer.ixx
│   ├── Scriptforge.StringConversion.ixx
│   ├── Scriptforge.ThreadError.ixx
│   ├── Scriptforge.ThrowError.ixx
│   ├── Scriptforge.Tree.ixx
│   ├── Scriptforge.Version.ixx
│   ├── Scriptforge.ixx
│   ├── detail
│   │   ├── Scriptforge.AntiDebug.RandomDefine.ixx
│   │   ├── Scriptforge.AntiDebug.cpp
│   │   └── Scriptforge.Define.hpp
│   ├── lang
│   │   ├── en.json
│   │   └── zh.json
│   ├── xmake.lua
│   └── 统计代码行数.ps1
├── scriptforge_test
│   ├── AntiDebug.Test.ixx
│   ├── Err.Test.ixx
│   ├── ErrCode.Test.ixx
│   ├── ErrCode.cpp
│   ├── Local.Test.ixx
│   ├── Msg.Test.ixx
│   ├── StringConversion.Test.ixx
│   ├── Test.cpp
│   ├── ThreadError.Test.ixx
│   ├── gmock_main.cpp
│   └── xmake.lua
├── token设计.md
├── xmake.lua
└── 更新方向.md
```

<!-- TREE_END -->