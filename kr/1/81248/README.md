# Контролна работа

## Оценка

1. `1/1`;

2. `0.9/1`;
    * (`-0.1`) трябваше да се извика `tr` с `exec(3)`, а не да се имплементира;

3. `0.3/1`;
    * задачата е имплементирана погрешно:
        * данните трябва да се обработват символ по символ, а не да се прочитат парчета с дължина на табулация в буфера и да се сравняват дали съдържат само интервали, тъй като така може да бъде пропуснато началото на табулация, ако то се пада някъде в средата на вече прочетения буфер;
    * получаваш въобще някакви точки за това, че:
        * (`+0.05`) има хенълдване на случая без командни аргументи;
        * (`+0.05`) има хенълдване на случая с поне един аргумент;
        * (`+0.1`) има някаква проверка дали в буфера има достатъчно интервали за табулация;
        * (`+0.1`) има някаква проверка дали има смисъл да се опитваме да прочетем необходимия брой символи в буфера за сравнението;

4. `1/1`.

Сумарно: `Много добър (5.2)`