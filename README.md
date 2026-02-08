# 42_ft_printf
🖨️ ft_printf – A custom implementation of the C printf function, built from the ground up at 42. 🎯 Handles formatted output with precision, efficiency, and full support for flags, width, and types.
# 🖨️ ft_printf - Your Custom Printf

> 🎯 **Recreate the famous printf function** - One of the most versatile C functions!

## 📖 What is ft_printf?

**ft_printf** is the second major project at 42 School. You'll build your own version of the standard `printf()` function. This teaches you:

- ✨ **Variadic functions** (functions with variable number of arguments)
- 🔄 **Format specifiers** parsing
- 🧮 **Number base conversions**

---

## 🎮 Supported Format Specifiers

| Specifier | Description | Example |
|:---------:|-------------|---------|
| `%c` | Single character | `ft_printf("%c", 'A')` → `A` |
| `%s` | String | `ft_printf("%s", "Hello")` → `Hello` |
| `%p` | Pointer address | `ft_printf("%p", ptr)` → `0x7fff5fbff8ac` |
| `%d` | Signed decimal integer | `ft_printf("%d", -42)` → `-42` |
| `%i` | Signed integer | `ft_printf("%i", 42)` → `42` |
| `%u` | Unsigned decimal integer | `ft_printf("%u", 42)` → `42` |
| `%x` | Hex (lowercase) | `ft_printf("%x", 255)` → `ff` |
| `%X` | Hex (UPPERCASE) | `ft_printf("%X", 255)` → `FF` |
| `%%` | Percent sign | `ft_printf("%%")` → `%` |

---

## 📁 Project Structure

```
ft_printf/
├── 📄 ft_printf.h      # Header file with prototypes
├── 📄 ft_printf.c      # Main printf logic
├── 📄 utils_word.c     # Character & string functions
├── 📄 utils_num.c      # Number printing functions
├── 📄 utils_hex.c      # Hexadecimal & pointer functions
└── 📄 Makefile         # Build automation
```

---

## 🔧 How It Works

### Step 1: Variadic Functions
```c
#include <stdarg.h>

int ft_printf(const char *format, ...)
{
    va_list args;           // Declare argument list
    va_start(args, format); // Initialize with last known argument
    
    // Process arguments...
    int num = va_arg(args, int);  // Get next argument as int
    
    va_end(args);           // Clean up
}
```

### Step 2: Parse Format String
```
"Hello %s, you are %d years old"
        ↓           ↓
      string      integer
```

### Step 3: Handle Each Specifier
The function loops through the format string:
1. If `%` found → check next character for specifier type
2. Print the corresponding argument
3. Otherwise → print the character as-is

---

## 🛠️ Compilation

```bash
# Build the library
make

# Clean object files
make clean

# Full clean (remove library too)
make fclean

# Rebuild everything
make re
```

---

## 💡 Usage Example

```c
#include "ft_printf.h"

int main(void)
{
    int count;
    
    // Basic usage
    ft_printf("Hello, World!\n");
    
    // With format specifiers
    ft_printf("Character: %c\n", 'A');
    ft_printf("String: %s\n", "42 School");
    ft_printf("Integer: %d\n", 42);
    ft_printf("Unsigned: %u\n", 4294967295);
    ft_printf("Hex lower: %x\n", 255);
    ft_printf("Hex upper: %X\n", 255);
    
    // Pointer
    int x = 42;
    ft_printf("Pointer: %p\n", &x);
    
    // Return value = number of characters printed
    count = ft_printf("Printed %d chars\n", 10);
    ft_printf("Total: %d\n", count);
    
    return (0);
}
```

### Compile & Run:
```bash
make
gcc main.c -L. -lftprintf -o test
./test
```

---

## 📊 Return Value

`ft_printf` returns:
- ✅ **Number of characters printed** on success
- ❌ **-1** on error (write failure)

---

## 🎓 Key Concepts for Beginners

### 1. What are Variadic Functions?
Functions that accept a **variable number of arguments**:
```c
ft_printf("No args");           // 0 extra args
ft_printf("%d", 42);            // 1 extra arg
ft_printf("%d %s", 42, "hi");   // 2 extra args
```

### 2. Hexadecimal Conversion
| Decimal | Binary | Hexadecimal |
|---------|--------|-------------|
| 0 | 0000 | 0 |
| 10 | 1010 | A |
| 15 | 1111 | F |
| 255 | 11111111 | FF |

### 3. The `%p` Specifier
Prints memory address where a variable is stored:
```c
int x = 5;
ft_printf("%p", &x);  // Output: 0x7ffeeb4a
```

---

## ✅ Testing Checklist

Test these edge cases:

- [ ] `ft_printf(NULL)` - NULL format string
- [ ] `ft_printf("%s", NULL)` - NULL string argument
- [ ] `ft_printf("%d", 0)` - Zero
- [ ] `ft_printf("%d", -2147483648)` - INT_MIN
- [ ] `ft_printf("%d", 2147483647)` - INT_MAX
- [ ] `ft_printf("%u", 4294967295)` - UINT_MAX
- [ ] `ft_printf("%x", 0)` - Zero in hex
- [ ] `ft_printf("%%")` - Percent sign
- [ ] `ft_printf("%p", NULL)` - NULL pointer

---

## 🔍 Debugging Tips

```bash
# Compare with real printf
./test > your_output.txt
# Then compare line by line

# Check memory leaks
valgrind --leak-check=full ./test

# Check norminette
norminette *.c *.h
```

---

## 📚 Useful Resources

- 📖 `man 3 printf` - Official printf documentation
- 📖 `man stdarg` - Variadic macros documentation
- 🔢 [ASCII Table](https://www.asciitable.com/)
- 🧮 [Hex/Decimal Converter](https://www.rapidtables.com/convert/number/hex-to-decimal.html)

---

## 👤 Author

**malbreik** - 42 Abu Dhabi Student

---

## ⭐ Pro Tips

1. 🧪 **Test incrementally** - Start with `%c`, then `%s`, then numbers
2. 📊 **Compare return values** with real `printf`
3. 🔄 **Handle edge cases** - Empty strings, NULL, negative numbers
4. 🧹 **Keep it clean** - One function = one responsibility
5. ✨ **Norminette first** - Check style before logic

---

*Made with ❤️ at 42 School*
