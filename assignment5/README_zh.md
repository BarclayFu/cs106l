<p align="center">
  <img src="docs/logo.jpeg" alt="Treebook 的 logo，一家虚构的斯坦福社交媒体创业公司" style="width: 300px; height: auto;" />
</p>

# 作业 5：Treebook

截止日期：5 月 15 日（周五）晚上 11:59

## 概览

斯坦福最新的社交媒体创业公司 Treebook 来了，你是创始团队成员！为了让产品顺利起步、与某家来自哈佛的完全无关联的不知名应用竞争，你被分配了实现用户个人资料（user profile）的任务。

在本次作业中，你将实现一个类的一部分，涉及运算符重载，以及修改某些方面的特殊成员函数（special member functions）。

你需要操作两个文件：

- `user.h` — 包含 `User` 类的声明，你需要在此添加特殊成员函数和运算符的声明
- `user.cpp` — 包含 `User` 类的定义/实现

要下载本次作业的初始代码，请参阅课程作业仓库的 [**Getting Started**](../README.md#getting-started) 说明。

## 运行代码

要运行代码，首先需要编译。打开终端（如果你使用 VSCode，按 <kbd>Ctrl+\`</kbd> 或进入顶部菜单 **Terminal > New Terminal**），确保你在 `assignment5/` 目录下，然后运行：

```sh
g++ -std=c++20 main.cpp user.cpp -o main
```

假设代码编译无错误，即可运行：

```sh
./main
```

这会执行 `main.cpp` 中的 `main` 函数。

在按照以下说明操作的过程中，建议你间歇编译/用自动评分器测试，以确保方向的正确！

> [!NOTE]
>
> ### Windows 用户注意
>
> 在 Windows 上，你可能需要使用以下命令编译：
>
> ```sh
> g++ -static-libstdc++ -std=c++20 main.cpp user.cpp -o main
> ```
>
> 才能正常看到输出。此外，生成的可执行文件可能叫 `main.exe`，你需要用以下命令运行：
>
> ```sh
> ./main.exe
> ```

## 第一部分：查看个人资料（Viewing Profiles）

看一下 `user.h` 头文件。你的同事们已经开始编写一个 `User` 类，用于存储每个加入你社交媒体平台的用户的名字和好友列表。为了让这个类超高效，他们选择用原始指针数组（`std::string` 的裸指针）来表示好友列表（类似于 `std::vector` 背后存储元素的方式）。好在他们已经写好了创建新 `User` 以及向已有 `User` 添加好友（`add_friend`）的逻辑，但在操作 `User` 对象时，他们开始遇到一些奇怪的问题。

首先，目前还没有一种简单的方式能把每个 `User` 对象的信息打印到控制台，这使得 Treebook 的调试变得困难。为了帮同事一把，请编写一个 `operator<<` 方法，将 `User` 打印到 `std::ostream`。**该运算符应在 `user.h` 中声明为友元函数，并在 `user.cpp` 中实现。** 例如，一个名为 `"Alice"`、好友为 `"Bob"` 和 `"Charlie"` 的用户，打印到控制台时应输出：

```
User(name=Alice, friends=[Bob, Charlie])
```

注意：`operator<<` 不应打印任何换行符。

> [!IMPORTANT]
> 在实现 `operator<<` 时，你需要访问并遍历 `User` 类的 `_friends` 私有字段才能打印出用户的好友。通常情况下，非成员函数无法访问类的私有字段——但我们可以通过将 `operator<<` 标记为 **`User` 类的友元函数**来绕过这一限制。详情请参阅周二课程的幻灯片！

## 第二部分：不友好的行为（Unfriendly Behaviour）

借助你的 `operator<<`，同事们在社交媒体应用的开发上取得了不错的进展。然而，当他们尝试在内存中复制 `User` 对象时，遇到了一些看似匪夷所思的问题。作为刚学过 CS106L 的你，怀疑这可能与 `User` 类的特殊成员函数（SMFs）有关——或者说，缺了一些必要的 SMF。为了解决这个问题，我们将为 `User` 类实现自定义版本的特殊成员函数，并删除（delete）那些编译器生成的默认版本不够用的函数。

具体来说，你需要：

1. 为 `User` 类实现析构函数，即实现 `~User()` SMF。
2. 让 `User` 类可拷贝构造（copy constructible），即实现 `User(const User& user)` SMF。
3. 让 `User` 类可拷贝赋值（copy assignable），即实现 `User& operator=(const User& user)` SMF。
4. 禁止 `User` 类被移动构造（move construct），即 delete `User(User&& user)` SMF。
5. 禁止 `User` 类被移动赋值（move assign），即 delete `User& operator=(User&& user)` SMF。

完成这些任务时，你需要修改 **两个** 文件：`user.h` 和 `user.cpp`。

> [!IMPORTANT]
> 在上面的第 2 点和第 3 点的实现中，你需要拷贝 `_friends` 数组的内容。回顾周四关于特殊成员函数的课程：拷贝指针数组时，你需要先为新数组分配内存（可以在成员初始化列表中完成），然后用循环将元素逐个拷贝过去。
> 还要确保同时设置你正在修改的实例的 `_size`、`_capacity` 和 `_name`！

## 第三部分：永远在加好友（Always Be Friending）

在修改了特殊成员函数之后，Treebook 已经能够在整个斯坦福推广，而且消息开始在其他大学传播开来！然而，你和你的同事们发现，`User` 类目前的一些常见使用场景要么不够方便、要么根本无法实现。你认为可以通过实现一些自定义运算符来解决这个问题。

你将为 `User` 类重载两个运算符。**请将这两个运算符都实现为成员函数**（即在 `user.h` 中 `User` 类内部声明，在 `user.cpp` 中提供实现）。

### `operator+=`

`+=` 运算符表示将一个用户添加到另一个用户的好友列表中。这应该是对称的（symmetric），也就是说，例如将 Charlie 添加到 Alice 的好友列表后，Alice 也应该出现在 Charlie 的好友列表中。考虑以下代码：

```cpp
User alice("Alice");
User charlie("Charlie");

alice += charlie;
std::cout << alice << std::endl;
std::cout << charlie << std::endl;

// 预期输出：
// User(name=Alice, friends=[Charlie])
// User(name=Charlie, friends=[Alice])
```

该运算符的函数签名应为 `User& operator+=(User& rhs)`。注意，和拷贝赋值运算符一样，它返回指向自身的引用。

### `operator<`

回忆一下，`<` 运算符是将用户存入 `std::set` 所必需的，因为 `std::set` 是基于比较运算符实现的。实现 `operator<`，按照名字的字母顺序比较用户。例如：

```cpp
User alice("Alice");
User charlie("Charlie");

if (alice < charlie)
  std::cout << "Alice is less than Charlie";
else
  std::cout << "Charlie is less than Alice";

// 预期输出：
// Alice is less than Charlie
```

该运算符的函数签名应为 `bool operator<(const User& rhs) const`。

## 🚀 提交说明

如果通过所有测试，就可以提交了！提交作业的步骤：

1. 请先完成 [此链接](https://forms.gle/tfLJSKnuUbUx9Xdi6) 的反馈问卷。
2. 在 [Paperless](https://paperless.stanford.edu) 上提交你的作业！

你需要提交的文件：

- `user.h`
- `user.cpp`

截止日期前你可以无限次重新提交。
