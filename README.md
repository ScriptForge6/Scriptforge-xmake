
<div align="center">

  <img src="assets/ScriptForge.png" alt="Scriptforge" width="100"/>
  
  <h1>Scriptforge</h1>
  
  <h2>An open‑source library packed with all sorts of quirky features</h2>

English | [简体中文](README‑CN.md)

[Changelog](CHANGELOG.md)

</div>

---

## ⚠️ Maintenance Notice

This library is currently undergoing major refactoring and updates.
Please note:
- **Compilation Errors**: The current version may contain compilation errors
- **API Stability**: API stability is not guaranteed during this period
- **Partial Features**: Some functions may be temporarily unavailable

We recommend using the latest stable release until the refactor is finished.

---

## Important Notes

This project targets the **C++23** standard. Compiler support for C++23 remains inconsistent across toolchains, so this library is not usable with certain compilers. Compilation success is prioritized for **MSVC**, which is also the author’s daily main‑compiler. GCC and Clang support will be added incrementally alongside compiler‑ and project‑level improvements and is not guaranteed.

---

## Quick Start

- [**AntiDebug**](#antidebug)
    - [**Basic Usage**](#basic-usage)
    - [**Custom Anti‑Cheat Detection & Self‑Termination Logic**](#custom-anti-cheat-detection--self-termination-logic)
- [**Argv**](#argv)
- [**Base64**](#base64)
    - [**Basic Base64 Usage**](#basic-base64-usage)
    - [**BasicBase64 for Custom Encoding Formats (Not Recommended)**](#basicbase64-for-custom-encoding-formats-not-recommended)
- [**Err**](#err)
    - [**Use Error Directly**](#use-error-directly)
    - [**Use BasicError**](#use-basicerror)
- [**LanguageCode**](#languagecode)
- [**Local**](#local)
    - [**Language Enum**](#language-enum)
    - [**LANG_NAME_TO_ENUM**](#lang_name_to_enum)
    - [**ISO639_1_TO_ENUM**](#iso639_1_to_enum)
    - [**ENUM_TO_LANG_NAME**](#enum_to_lang_name)
    - [**ENUM_TO_ISO639_1**](#enum_to_iso639_1)
- [**Msg**](#msg)
    - [**Basic Message Usage**](#basic-message-usage)
    - [**InformationLevel**](#informationlevel)
- [**Pch**](#pch)

### AntiDebug

`AntiDebug` type alias:

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
> - The destructor automatically calls `stop`.
> - Calling `start` blocks the main thread for `100 ms`.

#### Custom Anti‑Cheat Detection & Self‑Termination Logic

```
import Scriptforge.AntiDebug;
import Scriptforge.AntiDebug.RandomDefine;   // Expands obfuscation macros

class AntiDebugger : public Scriptforge::AntiDebug::AntiDebugger {
private:
    // Anti‑cheat detection logic
    bool F4() noexcept override {
          //TODO
        return is_attached;
    }
    // Self‑termination logic
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
> - Destructor automatically invokes `stop`.
> - `start` blocks the main thread for `100 ms`.
> - Module `Scriptforge.AntiDebug.RandomDefine` **must** be imported to expand function‑name macros.

---

### Argv

> 
> **Pending refactor; refer to source code for authoritative behavior**

`Argv` definition:

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
        std::vector<std::string>::iterator it, // Points to current command, for argument access
        std::string_view arg,                  // Invoked command name
        std::ostream& os,                      // Output stream
        std::ostream& err,                     // Error‑output stream
        std::istream& is                       // Input stream
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
> - The three streams permit input‑output redirection.
> - `FNV_1a_32` is built‑in; you may directly instantiate `Scriptforge::Argv::ArgvCli<>`.
> - You may also initialize via the `init()` member function.

---

### Base64

#### Basic Base64 Usage

Type alias definition:

```
using Scriptforge::Base::Base64 = Scriptforge::Base::BasicBase64<CharSet::Base64Set>;
```

Example:

```
import Scriptforge.Base64;
import Scriptforge.Local;

int main() {
    std::vector<unsigned char> raw = { 'H','e','l','l','o',',',' ','W','o','r','l','d','!' };
    Scriptforge::Base::Base64 b64_1{ raw/*, Scriptforge::Local::Lang{}*/ };
    std::cout << b64_1.encode() << std::endl;      // Output: SGVsbG8sIFdvcmxk==

    Scriptforge::Base::Base64 b64_2{ "SGVsbG8sIFdvcmxk=="/*, Scriptforge::Local::Lang{}*/ };
    std::vector<unsigned char> decode = b64_1.decode();
    std::cout
    << std::string(decode.begin(),decode.end())   // Output: Hello, World!
    << std::endl;
    return 0;
}
```

#### BasicBase64 for Custom Encoding Formats (Not Recommended)

`BasicBase64` definition:

```
template <Scriptforge::Base::isBaseCharSet CharSet>
class Scriptforge::Base::BasicBase64;
```

---

### Err

#### Use Error Directly

Type‑alias definition:

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
        std::cerr << e << std::endl;        // Output: [YYYY‑MM‑DD HH:MM:SS] [Error] [Code] Message
        std::cerr << e.code() << std::endl; // Output: Code
        std::cerr << e.what() << std::endl; // Output: Message
    }
}
```

> 
> - See [`Msg`](#informationlevel) for `Scriptforge::Msg::InformationLevel`.
> - Further APIs are documented under [`Msg`](#msg) (`Error` inherits from `Msg`).

#### Use BasicError

`BasicError` definition:

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
        std::cerr << e << std::endl;        // Case 1: Compile‑error, no stream‑operator overload for enum Error
                                            // Case 2: Output [YYYY‑MM‑DD HH:MM:SS] [Error] [0] Message
                                            // (Error1 implicitly converted to integer)
        e.code();                           // Returns: Error::Error1
        std::cerr << e.what() << std::endl; // Returns: Message
    }
}
```

---

### LanguageCode

> 
> Primarily used alongside [`Local`](#local).

#### Language Enum

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
> Full definitions reside in [`Scriptforge.LanguageCode.ixx`](scriptforge_lib/Scriptforge.LanguageCode.ixx).

#### LANG_NAME_TO_ENUM

> 
> Maintained for Win32‑API consumers to lower adoption friction.

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
> Full definitions reside in [`Scriptforge.LanguageCode.ixx`](scriptforge_lib/Scriptforge.LanguageCode.ixx).

#### ISO639_1_TO_ENUM

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
> Full definitions reside in [`Scriptforge.LanguageCode.ixx`](scriptforge_lib/Scriptforge.LanguageCode.ixx).

#### ENUM_TO_LANG_NAME

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
> Full definitions reside in [`Scriptforge.LanguageCode.ixx`](scriptforge_lib/Scriptforge.LanguageCode.ixx).

---

### Local

> 
> **Pending refactor; refer to source code for authoritative behavior**

#### Lang

> 
> No concise example available. Place JSON translation files named per ISO 639‑1 inside your language directory.

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

##### Basic Message Usage

Type‑alias definition:

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
    std::cout << msg << std::endl;           // Output: [YYYY‑MM‑DD HH:MM:SS] [Warning] Message
    msg.level();                             // Returns: Scriptforge::Msg::InformationLevel::Warning
    std::cout << msg.message() << std::endl; // Output: Message
    msg.time();                              // Returns std::chrono::system_clock::time_point at message creation time
    return 0;
}
```

#### InformationLevel

---

### Pch

Precompiled header. Import via `import "Scriptforge.Pch.hpp";`.

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