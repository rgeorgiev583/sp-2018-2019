# Контролна работа

## Оценка

1. `0/1`;

2. `1/1`;

3. `0.5/1`:
    * (`-0.1`) `SPACE_CHAR == buffer[0]` и `SPACE_CHAR != buffer[0]` са взаимно изключващи се, затова второто условие трябва да се замени с `else`;
    * (`-0.1`) `space_size` трябва да се занули, след като изведем съответния брой интервали при достигането на символ, който не е интервал;
    * (`-0.1`) не може да се присвоява стойност на статично алокиран масив: трябва да се ползва функцията `strcpy(3)` за целта (ако искаме да поставим низ вътре);
    * (`-0.05`) когато подаваме на `write(2)` някакъв буфер (в случая масив от символи), *не е* нужно да го реферираме чрез `&`;
    * (`-0.05`) когато подаваме на `write(2)` един символ , *е* нужно да го реферираме чрез `&`;
    * (`-0.05`) символните литерали (т.е. стойностите на `char`) се ограждат в единични, а не в двойни кавички;
    * (`-0.05`) подава се грешен размер (`2`) при извеждане на табулация (`\t`) чрез `read(2)`: размерът трябва да е `1`;

4. `1/1`.

Сумарно: `Много добър (4.5)`