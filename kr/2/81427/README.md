# Контролна работа

## Оценка

1. `0.7/1`;
    * (`-0.1`) няма проверка на броя на аргументите;
    * (`-0.1`) объркана е поредността на двата файла;
    * (`-0.1`) объркани са индексите на `argv`;

2. `0.5/1`:
    * (`-0.1`) няма проверка на броя на аргументите;
    * (`-0.1`) объркано е името на командата `tr`;
    * (`-0.1`) объркан е редът на аргументите на `dup2(2)`;
    * (`-0.1`) списъкът с аргументите на `execlp(3)` не е `NULL`-терминиран;
    * (`-0.1`) процесите, стартирани чрез `fork(2)`, не се изчакват накрая посредством `wait(2)`;

3. `0.3/1`:
    * задачата е имплементирана погрешно:
        * програмата изпада в безкраен цикъл;
    * получаваш въобще някакви точки за това, че:
        * (`+0.05`) има хенълдване на случая без командни аргументи;
        * (`+0.05`) има хенълдване на случая с поне един аргумент;
        * (`+0.1`) имплементирана е проверка за изобразимост на символа;
        * (`+0.1`) имплементирана е проверка за натрупване на минимален брой изобразими символи;

4. `0.7/1`:
    * (`-0.1`) подаден е грешният файлов дескриптор от канала на `dup2` за втората команда (т.е. `xargs kill`);
    * (`-0.1`) списъкът с аргументите на `execlp(3)` не е `NULL`-терминиран;
    * (`-0.05`) не се затварят накрая файловите дескриптори от канала в основния процес;
    * (`-0.05`) накрая се изчаква само единият процес да приключи, и то не този, на който искаме да вземем изходния код.

Сумарно: `Добър (4.2)`