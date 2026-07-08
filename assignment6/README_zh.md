# 作业 6：探索课程（Explore Courses）

截止日期：5 月 22 日（周五）晚上 11:59

## 概览

在本次作业中，你将练习对 `std::optional` 的理解。我们会用到和作业 1 中相同的 `courses.csv` 文件。你的任务是写一个函数：在 `CourseDatabase` 对象中尝试查找某个 `Course` 并将其返回。

同时你也会探索 `std::optional` 类附带的 monadic（单子式）操作。请先阅读代码、过一遍 `CourseDatabase` 类，弄懂它的接口。

## 运行代码

要运行代码，首先需要编译。打开终端（如果你使用 VSCode，按 <kbd>Ctrl+\`</kbd> 或进入顶部菜单 **Terminal > New Terminal**），确保你在 `assignment6/` 目录下，然后运行：

```sh
g++ -std=c++23 main.cpp -o main
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
> g++ -static-libstdc++ -std=c++23 main.cpp -o main
> ```
>
> 才能正常看到输出。此外，生成的可执行文件可能叫 `main.exe`，你需要用以下命令运行：
>
> ```sh
> ./main.exe
> ```

## 第 0 步：包含 `<optional>`

在 `main.cpp` 顶部加上 `#include <optional>`，本次作业中我们会用到 `std::optional`！

## 第一部分：实现 `find_course` 函数

该函数接收一个字符串 `course_title`，在 `CourseDatabase` 对象的私有成员 `courses` 中查找对应的课程。返回类型应该是什么？（提示：传入的 `course_title` 可能存在也可能不存在对应的 `Course`）

> [!NOTE]
> 你需要修改 `find_course` 的返回类型，当前代码中写的是 `FillMeIn`。

## 第二部分：修改 `main` 函数

注意 `main` 函数中调用了 `find_course`：

```cpp
auto course = db.find_course(argv[1]);
```

现在，你需要利用 [monadic 操作](https://en.cppreference.com/w/cpp/utility/optional) 来正确填充 `output` 字符串。我们一步步来看怎么做。

以下是你要复现的行为，**但不能使用任何条件判断**（比如 `if` 语句）：

```cpp
if (course.has_value()) {
    std::cout << "Found course: " << course->title << ","
            << course->number_of_units << "," << course->quarter << "\n";
} else {
    std::cout << "Course not found.\n";
}
```

简单来说，如果课程存在，则 `main` 底部的这行代码：

```cpp
std::cout << output << std::end;
```

应输出：

```bash
Found course: <title>,<number_of_units>,<quarter>
```

如果课程不存在，则应输出：

```bash
Course not found.
```

### Monadic 操作

一共有三种 monadic 操作：[`and_then`](https://en.cppreference.com/w/cpp/utility/optional/and_then)、[`transform`](https://en.cppreference.com/w/cpp/utility/optional/transform) 和 [`or_else`](https://en.cppreference.com/w/cpp/utility/optional/or_else)。请阅读课程幻灯片中对每种操作的说明，以及 [标准库文档](https://en.cppreference.com/w/cpp/utility/optional)。你只需要用到其中两种 monadic 操作。

你的代码最终应该类似这样：

```cpp
std::string output = course
    ./* monadic 函数一 */ (/* ... */)
    ./* monadic 函数二 */ (/* ... */)
    .value();                                  // 或者用 `.value_or(...)`，见下文
```

**从 `output` 的类型倒推回去思考**会很有帮助。注意每个 monadic 函数的作用，如下方提示所述。

> [!NOTE]
> 回顾每个 monadic 函数的作用。官方 C++ 库文档对这些函数的解释不太直观，所以我们在这里提供一个简短的参考。假设 `T` 和 `U` 是任意类型。
>
> ```cpp
> /** 
>  * 简而言之：
>  * 如果有值，调用函数生成一个新的 optional；否则，返回空（nothing）。
>  *
>  * 传给 `and_then` 的函数接受一个非 optional 的 T 类型实例，返回一个 std::optional<U>。
>  * 如果 optional 中有值，`and_then` 将函数应用于该值并返回结果。
>  * 如果 optional 中没有值（即 std::nullopt），则返回 std::nullopt。
>  */
> template <typename U>
> std::optional<U> std::optional<T>::and_then(std::function<std::optional<U>(T)> func);
> 
> /**
>  * 简而言之：
>  * 如果有值，对存储的值应用函数并将结果包装在一个 optional 中；否则，返回空。
>  *
>  * 传给 `transform` 的函数接受一个非 optional 的 T 类型实例，返回一个非 optional 的 U 类型实例。
>  * 如果 optional 中有值，`transform` 将函数应用于该值，并将结果包装在 std::optional<U> 中返回。
>  * 如果 optional 中没有值（即 std::nullopt），则返回 std::nullopt。
>  */
> template <typename U>
> std::optional<U> std::optional<T>::transform(std::function<U(T)> func);
> 
> /** 
>  * 简而言之：
>  * 如果有值，返回 optional 本身；否则，调用函数生成一个新的 optional。
>  *
>  * 与 `and_then` 相反。
>  * 传给 `or_else` 的函数不接受任何参数，返回一个 std::optional<U>。
>  * 如果 optional 中有值，`or_else` 直接返回它。
>  * 如果 optional 中没有值（即 std::nullopt），`or_else` 调用函数并返回结果。
>  */
> template <typename U>
> std::optional<U> std::optional<T>::or_else(std::function<std::optional<U>()> func);
> ```
>
> 举例来说，给定一个 `std::optional<T> opt` 对象，monadic 操作可以这样调用：
>
> ```cpp
> opt
>   .and_then([](T value) -> std::optional<U> { return /* ... */; })
>   .transform([](T value) -> U { return /* ... */; })
>   .or_else([]() -> std::optional<U> { return /* ... */; });
> ```
>
> <sup>注意：lambda 函数中的 `->` 语法是显式写出函数返回类型的方式！</sup>
>
> 注意，由于每个方法都返回一个 `std::optional`，你可以将它们链式调用。如果你确定链式调用结束时 optional 一定有值，可以调用 [`.value()`](https://en.cppreference.com/w/cpp/utility/optional/value) 来获取该值。否则，你可以调用 [`.value_or(fallback)`](https://en.cppreference.com/w/cpp/utility/optional/value_or)，在 optional 中没有值时返回一个备用的 `fallback` 值。

## 🚀 提交说明

如果通过所有测试，就可以提交了！提交作业的步骤：

1. 请先完成 [此链接](https://forms.gle/aGuFqLyhB18mNoPKA) 的反馈问卷。
2. 在 [Paperless](https://paperless.stanford.edu) 上提交你的作业！

你需要提交的文件：

- `main.cpp`

截止日期前你可以无限次重新提交。
