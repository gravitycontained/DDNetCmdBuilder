# DDNetCmdBuilder
creates command chains and copies them into your clipboard (CTRL + V)

default values (from default.cfg)
 - if command  is empty: "ban" is selected
 - if duration is empty: "5" is selected
 - if reason   is empty: "block" is selected

example:

```
command  > ban
reason   > blocking
duration > 10
ids      > 1, 5, 21

OUTPUT:
ban 1 10 blocking; ban 5 10 blocking; ban 21 10 blocking;
( copied to clipboard )
```
