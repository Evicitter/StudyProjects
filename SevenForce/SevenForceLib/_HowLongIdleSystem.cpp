//В Windows 2000 для этих целей предусмотрена системная функция GetLastInputInfo, которая заполняет структуру LASTINPUTINFO нужной нам информацией:

 LASTINPUTINFO lpi;
 lpi.cbSize = sizeof(lpi);
 GetLastInputInfo(&lpi);

 //lpi.dwTime
 
 //После вызова GetLastInputInfo, lpi.dwTime содержит количество миллисекунд, прошедших с момента прекращения каких либо действий пользователя. С одной стороны, это то, что нам нужно, но с другой - эта функция доступна только в Windows 2000 - но не в Windows 9x и не в Windows NT 4.0. Надо бы подумать, что в таком случае можно предпринять.
 
 