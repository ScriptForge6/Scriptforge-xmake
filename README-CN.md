<div align="center">

  <img src="assets/ScriptForge.png" alt="Scriptforge" width="100"/>
  
  <h1>Scriptforge</h1>
  
  <h2>一个有着许许多多奇奇怪怪功能的开源库</h2>

[English](README.md) | 简体中文

[更新日志](CHANGELOG-CN.md)

</div>

---

## ⚠️ 维护通知

本库目前正在进行重大重构和更新。
请注意：

- **编译错误**：当前版本可能会出现编译错误
- **API稳定性**：在此期间无法保证API稳定性
- **部分功能**：可能暂时不可用

我们建议在更新完成前使用最新的稳定版本。

---

## 重要消息

本项目使用`C++23`标准进行编程，鉴于不同编译器对于`C++23`标准的支持暂不统一，因此项目对部分编译器不可用，正常情况下，优先确保`MSVC`中编译成功，本人也使用`MSVC`作为日常主力编译器，对于`GCC`和`Clang`的支持会随着编译器和项目的更新缓步支持，不保证可以使用。

---

## 快速开始

- [**AntiDebug**](#antidebug)
    - [**直接使用**](#直接使用)
    - [**使用自己的反作弊检测逻辑和自杀逻辑**](#使用自己的反作弊检测逻辑和自杀逻辑)
- [**Argv**](#argv)
- [**Err**](#err)
    - [**直接使用Error**](#直接使用error)
    - [**使用BasicError**](#使用basicerror)
- [**LanguageCode**](#languagecode)
- [**Local**](#local)
    - [**Language枚举**](#language枚举)
    - [**LANG_NAME_TO_ENUM**](#lang_name_to_enum)
    - [**ISO639_1_TO_ENUM**](#iso639_1_to_enum)
    - [**ENUM_TO_LANG_NAME**](#enum_to_lang_name)
    - [**ENUM_TO_ISO639_1**](#enum_to_iso639_1)
- [**Msg**](#msg)
-   - [**直接使用Message**](#直接使用message)
    - [**InformationLevel**](#informationlevel)
- [**Pch**](#pch)

### AntiDebug

`AntiDebug`定义如下：

```cpp
using Scriptforge::AntiDebug::AntiDebugger = ADNS::ADCL;
```

> `ADNS`与`ADCL`皆为混淆宏。

#### 直接使用

```cpp
import Scriptforge.AntiDebug;

int main() {
    Scriptforge::AntiDebug::AntiDebugger debugger;
    debugger.start();

    //TODO

    debugger.stop();

    return 0;
}
```

> - 析构函数会自动调用`stop`函数。  
> - 调用`start`函数时，主线程会被堵塞`100ms`。

#### 使用自己的反作弊检测逻辑和自杀逻辑

```cpp
import Scriptforge.AntiDebug;
import Scriptforge.AntiDebug.RandomDefine;   //用于展开宏

class AntiDebugger : public Scriptforge::AntiDebug::AntiDebugger {
private:

    //反作弊检测逻辑
    bool F4() noexcept override {

	      //TODO

        return is_attached;
    }

    //自杀逻辑
    void F5() noexcept override {
			
        //TODO

    }
}

int main() {
    AntiDebug debugger;
    debugger.start();

    //TODO

    debugger.stop();
    return 0;
}

```

> - `F4`与`F5`皆为混淆宏。
> - 析构函数会自动调用`stop`函数。  
> - 调用`start`函数时，主线程会被堵塞`100ms`。
> - 必须导入`Scriptforge.AntiDebug.RandomDefine`模块，  
    以展开函数名宏。

---

### Argv

> **即将重构，以代码为准**

`Argv`定义如下：

```cpp
template<Scriptforge::Argv::isHash HashT = Scriptforge::Argv::Hash::FNV_1a_32>
    class Scriptforge::Argv::ArgvCli;
```

示例：

```cpp
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
        std::vector<std::string>::iterator it, //初始为当前命令位置，用于访问参数
        std::string_view arg,                  //使用的命令名称
        std::ostream& os,                      //输出流
        std::ostream& err,                     //错误输出流
        std::istream& is                       //输入流
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

> - 三个流用于重定向输入/输出。
> - 在项目内内置了`FNV_1a_32`，  
>   可以直接使用`Scriptforge::Argv::ArgvCli<>`定义。
> - 也可以使用`init`函数初始化。

---

### Err

#### 直接使用Error

`Error`定义如下：

```cpp
using Scriptforge::Err::Error = Scriptforge::Err::BasicError<>;
```

```cpp
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
        std::cerr << e << std::endl;        //输出：[YYYY-MM-DD HH:MM:SS] [Error] [Code] Message
        std::cerr << e.code() << std::endl; //输出：Code
        std::cerr << e.what() << std::endl; //输出：Message
    }
}
```
> - `Scriptforge::Msg::InformationLevel`的定义见[`Msg`](#informationlevel)
> - 其他函数可以见[`Msg`](#msg)（`Error`继承于`Msg`）

#### 使用BasicError

`BasicError`定义如下：

```cpp
template<typename CodeT = std::string, typename T = std::string, typename Clock = std::chrono::system_clock>
    requires Scriptforge::Err::ErrorRequires<CodeT, T, Clock>
class Scriptforge::Err::BasicError : public Scriptforge::Msg::BasicMessage<T, Clock>;
```

```cpp
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
        std::cerr << e << std::endl;        //可能一：报错，Error未重载流运算符
                                            //可能二：输出[YYYY-MM-DD HH:MM:SS] [Error] [0] Message
                                            //（Error1隐式转换成int）
        e.code();                           //返回：Error::Error1
        std::cerr << e.what() << std::endl; //返回：Message
    }
}
```

---

### LanguageCode

> 主要配合[`Local`](#local)使用。

#### Language枚举

`Language枚举`定义如下：

```cpp
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

> 定义详见[`Scriptforge.LanguageCode.ixx`](scriptforge_lib/Scriptforge.LanguageCode.ixx)。

#### LANG_NAME_TO_ENUM

> 为方便`Win32API`用户，我们维护了此表，用于快速上手。

`LANG_NAME_TO_ENUM`定义如下：

```cpp
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

> 定义详见[`Scriptforge.LanguageCode.ixx`](scriptforge_lib/Scriptforge.LanguageCode.ixx)。

#### ISO639_1_TO_ENUM

`ISO639_1_TO_ENUM`定义如下：

```cpp
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

> 定义详见[`Scriptforge.LanguageCode.ixx`](scriptforge_lib/Scriptforge.LanguageCode.ixx)。

#### ENUM_TO_LANG_NAME

`ENUM_TO_LANG_NAME`定义如下：

```cpp
inline const std::map<Language, std::string> ENUM_TO_LANG_NAME = []() {
    std::map<Language, std::string> m;
    for (const auto& pair : LANG_NAME_TO_ENUM) {
        m[pair.second] = pair.first;
    }
    return m;
}();
```

#### ENUM_TO_ISO639_1

`ENUM_TO_ISO639_1`定义如下：

```cpp
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

> 定义详见[`Scriptforge.LanguageCode.ixx`](scriptforge_lib/Scriptforge.LanguageCode.ixx)。

---

### Local

> **即将重构，以代码为准**

#### Lang

`Lang`定义如下（不好给示例，自己看吧，在文件夹里放以ISO 639-1命名的json文件）：

```cpp
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

##### 直接使用Message

`Message`定义如下：

```cpp
using Scriptforge::Msg::Message = Scriptforge::Msg::BasicMessage<>;
```

示例：

```cpp
import <iostream>;
import Scriptforge.Msg;

int main() {
    Scriptforge::Msg::Message msg{
        "Message",
        Scriptforge::Msg::InformationLevel::Warning
    };

    std::cout << msg << std::endl;           //输出：[YYYY-MM-DD HH:MM:SS] [Warning] Message
    msg.level();                             //返回：Scriptforge::Msg::InformationLevel::Warning
    std::cout << msg.message() << std::endl; //输出：Message
    msg.time();                              //返回注册时间点的 std::chrono::system_clock::time_point 对象
    
    return 0;
}
```

#### InformationLevel

---

### Pch

一个预编译头文件，可以使用`import "Scriptforge.Pch.hpp";`使用。

---

## 项目文件

> `GitHub Actions` 自动生成

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
│   ├── Scriptforge.AntiDebug.RandomDefine.ixx
│   ├── Scriptforge.AntiDebug.cpp
│   ├── Scriptforge.AntiDebug.ixx
│   ├── Scriptforge.Argv.ixx
│   ├── Scriptforge.Err.ixx
│   ├── Scriptforge.ErrCode.ixx
│   ├── Scriptforge.ErrCode.throwError.ixx
│   ├── Scriptforge.LanguageCode.ixx
│   ├── Scriptforge.Local.ixx
│   ├── Scriptforge.Log.ixx
│   ├── Scriptforge.Msg.ixx
│   ├── Scriptforge.Pch.hpp
│   ├── Scriptforge.ProcessPool.ixx
│   ├── Scriptforge.RingBuffer.ixx
│   ├── Scriptforge.StringConversion.ixx
│   ├── Scriptforge.ThreadError.ixx
│   ├── Scriptforge.Tree.ixx
│   ├── Scriptforge.Version.ixx
│   ├── Scriptforge.ixx
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
│   └── xmake.lua
├── xmake.lua
└── 更新方向.md
```

<!-- TREE_END -->