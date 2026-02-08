/* ************************************************************************** */
/*                         FT_PRINTF - SINGLE FILE VERSION                    */
/*                    All functionality combined for easy understanding       */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <limits.h>

/* ========================================================================== */
/*                            FUNCTION PROTOTYPES                             */
/* ========================================================================== */

int	ft_printf(const char *string, ...);
int	ft_printf_output(va_list flags, const char *string, int i, int *total);

// Basic output functions
int	ft_putchar(char c);
int	ft_putstr(char *s);

// Number output functions
int	ft_putnbr(int n);
int	ft_putnbr_u(unsigned int n);

// Hexadecimal output functions
int	ft_puthex(unsigned int n, char c, int *printed);
int	ft_putmem(size_t n, int *printed);

/* ========================================================================== */
/*                         BASIC OUTPUT FUNCTIONS                             */
/* ========================================================================== */

// Writes a single character to stdout
int	ft_putchar(char c)
{
	if (write(1, &c, 1) == -1)
		return (-1);
	return (1);
}

// Writes a string to stdout, handles NULL case
int	ft_putstr(char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
	{
		if (write(1, "(null)", 6) == -1)
			return (-1);
		return (6);
	}
	while (s[i])
	{
		if (ft_putchar(s[i]) == -1)
			return (-1);
		i++;
	}
	return (i);
}

/* ========================================================================== */
/*                         NUMBER OUTPUT FUNCTIONS                            */
/* ========================================================================== */

// Counts the number of digits in a number
static int	num_places(int n)
{
	int	r;

	r = 1;
	while (n > 9)
	{
		n /= 10;
		r++;
	}
	return (r);
}

// Counts the number of digits in an unsigned number
static int	num_places_u(unsigned int n)
{
	int	r;

	r = 1;
	while (n > 9)
	{
		n /= 10;
		r++;
	}
	return (r);
}

// Prints a signed integer
int	ft_putnbr(int n)
{
	int	digits;

	digits = 0;
	if (n == INT_MIN)
	{
		if (write(1, "-2147483648", 11) == -1)
			return (-1);
		return (11);
	}
	if (n < 0)
	{
		if (ft_putchar('-') == -1)
			return (-1);
		n = -n;
		digits++;
	}
	if (n >= 10)
	{
		if (ft_putnbr(n / 10) == -1)
			return (-1);
	}
	if (ft_putchar(n % 10 + '0') == -1)
		return (-1);
	digits += num_places(n);
	return (digits);
}

// Prints an unsigned integer
int	ft_putnbr_u(unsigned int n)
{
	int	digits;

	digits = 0;
	if (n >= 10)
	{
		if (ft_putnbr(n / 10) == -1)
			return (-1);
	}
	if (ft_putchar(n % 10 + '0') == -1)
		return (-1);
	digits += num_places_u(n);
	return (digits);
}

/* ========================================================================== */
/*                      HEXADECIMAL OUTPUT FUNCTIONS                          */
/* ========================================================================== */

// Returns the correct base (lowercase or uppercase hex)
static char	*check_base(char c)
{
	if (c == 'x')
		return ("0123456789abcdef");
	else
		return ("0123456789ABCDEF");
}

// Prints an unsigned int in hexadecimal format
int	ft_puthex(unsigned int num, char c, int *printed)
{
	char	*base;
	int		i;
	char	string[25];

	i = 0;
	base = check_base(c);
	if (num == 0)
	{
		if (ft_putchar('0') == -1)
			return (-1);
		return (1);
	}
	while (num != 0)
	{
		string[i++] = base[num % 16];
		num /= 16;
	}
	*printed += i;
	while (i--)
	{
		if (ft_putchar(string[i]) == -1)
			return (-1);
	}
	return (0);
}

// Helper function to handle zero case for memory address
static int	check_zero_mem(size_t num, int *p)
{
	if (num == 0)
	{
		if (ft_putchar('0') == -1)
			return (-1);
		*p += 1;
	}
	return (0);
}

// Prints a memory address in hexadecimal format (with 0x prefix)
int	ft_putmem(size_t num, int *printed)
{
	char	*base;
	int		i;
	char	string[25];

	i = 0;
	base = "0123456789abcdef";
	if (write(1, "0x", 2) == -1)
		return (-1);
	*printed += 2;
	if (check_zero_mem(num, printed) == -1)
		return (-1);
	while (num != 0)
	{
		string[i++] = base[num % 16];
		num /= 16;
		(*printed)++;
	}
	while (i--)
	{
		if (ft_putchar(string[i]) == -1)
			return (-1);
	}
	return (0);
}

/* ========================================================================== */
/*                         MAIN FT_PRINTF FUNCTIONS                           */
/* ========================================================================== */

// Handles the format specifier and prints the corresponding argument
int	ft_printf_output(va_list flags, const char *string, int i, int *total)
{
	int	printed;

	printed = 0;
	if (string[i] == 'c')
		printed = ft_putchar(va_arg(flags, int));
	else if (string[i] == 's')
		printed = ft_putstr(va_arg(flags, char *));
	else if (string[i] == 'd' || string[i] == 'i')
		printed = ft_putnbr(va_arg(flags, int));
	else if (string[i] == 'u')
		printed = ft_putnbr_u(va_arg(flags, unsigned int));
	else if (string[i] == 'x' || string[i] == 'X')
		printed = ft_puthex(va_arg(flags, unsigned int), string[i], total);
	else if (string[i] == 'p')
		printed = ft_putmem(va_arg(flags, size_t), total);
	else if (string[i] == '%')
		printed = ft_putchar('%');
	if (printed == -1)
		return (-1);
	if (string[i] != 'x' && string[i] != 'X' && string[i] != 'p')
		*total += printed;
	return (0);
}

// Main printf function - parses format string and prints formatted output
int	ft_printf(const char *string, ...)
{
	va_list	flags;
	int		i;
	int		total_chars;

	i = 0;
	total_chars = 0;
	va_start(flags, string);
	while (string[i] != '\0')
	{
		if (string[i] == '%')
		{
			i++;
			if (ft_printf_output(flags, string, i, &total_chars) == -1)
				return (-1);
		}
		else
		{
			if (ft_putchar(string[i]) == -1)
				return (-1);
			total_chars++;
		}
		i++;
	}
	va_end(flags);
	return (total_chars);
}

/* ========================================================================== */
/*                              TEST MAIN                                     */
/* ========================================================================== */

#include <stdio.h>

int main(void)
{
	int ret1, ret2;
	
	printf("\n=== TESTING FT_PRINTF ===\n\n");
	
	// Test %c (character)
	printf("--- Character (%%c) ---\n");
	ret1 = printf("Real:  [%c]\n", 'A');
	ret2 = ft_printf("Mine:  [%c]\n", 'A');
	printf("Return: real=%d, mine=%d\n\n", ret1, ret2);
	
	// Test %s (string)
	printf("--- String (%%s) ---\n");
	ret1 = printf("Real:  [%s]\n", "Hello World!");
	ret2 = ft_printf("Mine:  [%s]\n", "Hello World!");
	printf("Return: real=%d, mine=%d\n\n", ret1, ret2);
	
	// Test %s with NULL
	printf("--- NULL String ---\n");
	ret1 = printf("Real:  [%s]\n", (char *)NULL);
	ret2 = ft_printf("Mine:  [%s]\n", (char *)NULL);
	printf("Return: real=%d, mine=%d\n\n", ret1, ret2);
	
	// Test %d (integer)
	printf("--- Integer (%%d) ---\n");
	ret1 = printf("Real:  [%d] [%d] [%d]\n", 42, -42, 0);
	ret2 = ft_printf("Mine:  [%d] [%d] [%d]\n", 42, -42, 0);
	printf("Return: real=%d, mine=%d\n\n", ret1, ret2);
	
	// Test %i (integer)
	printf("--- Integer (%%i) ---\n");
	ret1 = printf("Real:  [%i]\n", 12345);
	ret2 = ft_printf("Mine:  [%i]\n", 12345);
	printf("Return: real=%d, mine=%d\n\n", ret1, ret2);
	
	// Test %u (unsigned)
	printf("--- Unsigned (%%u) ---\n");
	ret1 = printf("Real:  [%u]\n", 4294967295u);
	ret2 = ft_printf("Mine:  [%u]\n", 4294967295u);
	printf("Return: real=%d, mine=%d\n\n", ret1, ret2);
	
	// Test %x (hex lowercase)
	printf("--- Hex lowercase (%%x) ---\n");
	ret1 = printf("Real:  [%x] [%x]\n", 255, 0);
	ret2 = ft_printf("Mine:  [%x] [%x]\n", 255, 0);
	printf("Return: real=%d, mine=%d\n\n", ret1, ret2);
	
	// Test %X (hex uppercase)
	printf("--- Hex uppercase (%%X) ---\n");
	ret1 = printf("Real:  [%X]\n", 255);
	ret2 = ft_printf("Mine:  [%X]\n", 255);
	printf("Return: real=%d, mine=%d\n\n", ret1, ret2);
	
	// Test %p (pointer)
	printf("--- Pointer (%%p) ---\n");
	int x = 42;
	ret1 = printf("Real:  [%p]\n", (void *)&x);
	ret2 = ft_printf("Mine:  [%p]\n", (void *)&x);
	printf("Return: real=%d, mine=%d\n\n", ret1, ret2);
	
	// Test %% (percent)
	printf("--- Percent (%%%%) ---\n");
	ret1 = printf("Real:  [%%]\n");
	ret2 = ft_printf("Mine:  [%%]\n");
	printf("Return: real=%d, mine=%d\n\n", ret1, ret2);
	
	// Test complex format
	printf("--- Complex Format ---\n");
	ret1 = printf("Real:  Name: %s, Age: %d, Score: %u%%\n", "John", 25, 95);
	ret2 = ft_printf("Mine:  Name: %s, Age: %d, Score: %u%%\n", "John", 25, 95);
	printf("Return: real=%d, mine=%d\n\n", ret1, ret2);
	
	return (0);
}

/* ========================================================================== */
/*                              PSEUDOCODE                                    */
/* ========================================================================== */

/*

===============================================================================
                    HOW FT_PRINTF WORKS - PSEUDOCODE EXPLANATION
===============================================================================

┌─────────────────────────────────────────────────────────────────────────┐
│  1. MAIN FUNCTION: ft_printf()                                          │
└─────────────────────────────────────────────────────────────────────────┘

PSEUDOCODE:
-----------
FUNCTION ft_printf(format_string, variable_arguments...):
    
    SETUP:
        - Initialize variable_argument_list (va_list)
        - Set index = 0
        - Set total_chars = 0
    
    MAIN LOOP (go through format string character by character):
        WHILE format_string[index] is not '\0':
            
            IF current character is '%':
                index++  // Move to the specifier (c, s, d, etc.)
                Call handle_format_specifier(args, format_string, index, &total)
                IF error: return -1
            
            ELSE:
                Print the regular character
                total_chars++
            
            index++  // Move to next character
    
    CLEANUP variable_argument_list
    RETURN total_chars

EXAMPLE:
--------
Input: ft_printf("Hello %s!\n", "World")

Step 1: Print 'H' → total = 1
Step 2: Print 'e' → total = 2
Step 3: Print 'l' → total = 3
Step 4: Print 'l' → total = 4
Step 5: Print 'o' → total = 5
Step 6: Print ' ' → total = 6
Step 7: Found '%', move to 's'
Step 8: Handle %s → Get argument "World" → Print "World" → total = 11
Step 9: Print '!' → total = 12
Step 10: Print '\n' → total = 13

Output: "Hello World!\n"
Return: 13


┌─────────────────────────────────────────────────────────────────────────┐
│  2. FORMAT HANDLER: ft_printf_output()                                  │
└─────────────────────────────────────────────────────────────────────────┘

PSEUDOCODE:
-----------
FUNCTION handle_format_specifier(args, format_string, index, total):
    
    current_specifier = format_string[index]
    
    SWITCH current_specifier:
        CASE 'c':  // Character
            arg = va_arg(args, int)
            printed = ft_putchar(arg)
        
        CASE 's':  // String
            arg = va_arg(args, char*)
            printed = ft_putstr(arg)
        
        CASE 'd' OR 'i':  // Signed integer
            arg = va_arg(args, int)
            printed = ft_putnbr(arg)
        
        CASE 'u':  // Unsigned integer
            arg = va_arg(args, unsigned int)
            printed = ft_putnbr_u(arg)
        
        CASE 'x' OR 'X':  // Hexadecimal
            arg = va_arg(args, unsigned int)
            printed = ft_puthex(arg, specifier, total)
        
        CASE 'p':  // Pointer
            arg = va_arg(args, size_t)
            printed = ft_putmem(arg, total)
        
        CASE '%':  // Literal %
            printed = ft_putchar('%')
    
    IF error: return -1
    Update *total with printed count
    RETURN 0


┌─────────────────────────────────────────────────────────────────────────┐
│  3. PRINTING INTEGERS: ft_putnbr()                                      │
└─────────────────────────────────────────────────────────────────────────┘

ALGORITHM: Recursive printing (left to right)
---------------------------------------------

FUNCTION print_integer(number):
    
    // Special case: most negative number
    IF number == -2147483648:
        Print "-2147483648"
        RETURN 11
    
    // Handle negative
    IF number < 0:
        Print '-'
        number = -number (make positive)
        digits++
    
    // Recursive step: print all digits except the last
    IF number >= 10:
        print_integer(number / 10)  // Print left part
    
    // Base case: print the last digit
    Print (number % 10) + '0'
    
    RETURN digit_count

EXAMPLE: print_integer(1234)
----------------------------
Call 1: print_integer(1234)
    → 1234 >= 10, so call print_integer(123)
    
    Call 2: print_integer(123)
        → 123 >= 10, so call print_integer(12)
        
        Call 3: print_integer(12)
            → 12 >= 10, so call print_integer(1)
            
            Call 4: print_integer(1)
                → 1 < 10, skip recursion
                → Print '1' (1 % 10 + '0')
                → Return
            
            → Print '2' (12 % 10 + '0')
            → Return
        
        → Print '3' (123 % 10 + '0')
        → Return
    
    → Print '4' (1234 % 10 + '0')
    → Return

Output: "1234"


┌─────────────────────────────────────────────────────────────────────────┐
│  4. HEXADECIMAL CONVERSION: ft_puthex()                                 │
└─────────────────────────────────────────────────────────────────────────┘

ALGORITHM: Base-16 conversion with digit reversal
-------------------------------------------------

FUNCTION print_hex(number, specifier):
    
    base = (specifier == 'x') ? "0123456789abcdef" : "0123456789ABCDEF"
    buffer[25]
    index = 0
    
    // Special case
    IF number == 0:
        Print '0'
        RETURN 1
    
    // Convert to hex (stores digits in REVERSE)
    WHILE number != 0:
        remainder = number % 16
        buffer[index] = base[remainder]
        index++
        number = number / 16
    
    // Print digits in CORRECT order (reverse the buffer)
    WHILE index > 0:
        index--
        Print buffer[index]
    
    RETURN count

EXAMPLE: print_hex(255, 'x')
----------------------------
Step 1: 255 % 16 = 15 → buffer[0] = 'f', number = 15
Step 2: 15 % 16 = 15 → buffer[1] = 'f', number = 0
Step 3: number == 0, stop

Buffer: ['f', 'f']
        ↑    ↑
      index=0 index=1

Now print in reverse:
Step 4: Print buffer[1] = 'f'
Step 5: Print buffer[0] = 'f'

Output: "ff"


┌─────────────────────────────────────────────────────────────────────────┐
│  5. POINTER ADDRESS: ft_putmem()                                        │
└─────────────────────────────────────────────────────────────────────────┘

PSEUDOCODE:
-----------
FUNCTION print_memory_address(address):
    
    Print "0x"  // Prefix for all pointers
    
    IF address == 0:
        Print '0'
        RETURN
    
    // Same as hex conversion but with memory address
    WHILE address != 0:
        buffer[index] = hex_digit(address % 16)
        index++
        address = address / 16
    
    // Print digits in reverse
    WHILE index > 0:
        index--
        Print buffer[index]


┌─────────────────────────────────────────────────────────────────────────┐
│  6. COMPLETE EXAMPLE WALKTHROUGH                                        │
└─────────────────────────────────────────────────────────────────────────┘

CALL: ft_printf("User: %s, ID: %d, Level: 0x%X\n", "Alice", 42, 255)

STEP-BY-STEP EXECUTION:
-----------------------

1. index=0, total=0
2. 'U' → Print 'U', total=1
3. 's' → Print 's', total=2
4. 'e' → Print 'e', total=3
5. 'r' → Print 'r', total=4
6. ':' → Print ':', total=5
7. ' ' → Print ' ', total=6

8. '%' → Found specifier!
9. 's' → Handle %s:
   - Get argument: "Alice"
   - Call ft_putstr("Alice")
   - Print: 'A', 'l', 'i', 'c', 'e'
   - Returns 5
   - total=11

10. ',' → Print ',', total=12
11. ' ' → Print ' ', total=13
12. 'I' → Print 'I', total=14
13. 'D' → Print 'D', total=15
14. ':' → Print ':', total=16
15. ' ' → Print ' ', total=17

16. '%' → Found specifier!
17. 'd' → Handle %d:
    - Get argument: 42
    - Call ft_putnbr(42)
    - Prints: '4', '2'
    - Returns 2
    - total=19

18. ',' → Print ',', total=20
19. ' ' → Print ' ', total=21
20. 'L' → Print 'L', total=22
21. 'e' → Print 'e', total=23
22. 'v' → Print 'v', total=24
23. 'e' → Print 'e', total=25
24. 'l' → Print 'l', total=26
25. ':' → Print ':', total=27
26. ' ' → Print ' ', total=28
27. '0' → Print '0', total=29
28. 'x' → Print 'x', total=30

29. '%' → Found specifier!
30. 'X' → Handle %X:
    - Get argument: 255
    - Call ft_puthex(255, 'X', &total)
    - Convert: 255 → "FF"
    - Prints: 'F', 'F'
    - total=32

31. '\n' → Print newline, total=33
32. '\0' → End of string

FINAL OUTPUT: "User: Alice, ID: 42, Level: 0xFF\n"
RETURN VALUE: 33


┌─────────────────────────────────────────────────────────────────────────┐
│  7. KEY CONCEPTS SUMMARY                                                │
└─────────────────────────────────────────────────────────────────────────┘

1. VARIADIC FUNCTIONS:
   - Accept variable number of arguments
   - Use: va_list, va_start, va_arg, va_end
   - va_arg(list, type) extracts next argument

2. FORMAT STRING PARSING:
   - Loop through string character by character
   - Regular char → print it
   - '%' → trigger format handler

3. RECURSIVE NUMBER PRINTING:
   - Print left digits first (recursion)
   - Then print rightmost digit
   - Ensures left-to-right output

4. BASE CONVERSION:
   - Repeatedly: digit = number % base, number /= base
   - Stores digits in reverse order
   - Print buffer backwards for correct order

5. ERROR HANDLING:
   - Check write() return value (-1 on error)
   - Propagate errors up the call stack
   - Return -1 on any failure

6. CHARACTER COUNTING:
   - Each function returns chars printed
   - Main function accumulates total
   - Return total at the end

===============================================================================
*/
