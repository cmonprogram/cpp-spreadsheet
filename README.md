# cpp-spreadsheet
Проект электронная таблица, позволяет задавать и вычислять формулы, заданные в ячейках по аналогии с Excel

## Развертывание
1. Устновить java
2. Скачать (или собрать из исходников) и положить в папку с проектом файлы (https://www.antlr.org):
```
antlr4_runtime
antlr-4.13.1-complete.jar
```
3.  Собрать проект
```
cmake . -DCMAKE_BUILD_TYPE=Debug && cmake --build .
./spreadsheet
```
# Формат входных данных
Команда sheet->SetCell устанавливает в ячейку значение формулы или текста
# Формат выходных данных
sheet->PrintTexts выводит в поток ячейки таблицы с текстом формул.

sheet->PrintValues выводит в поток значения формул
## Использование
### Ввод
```
int main() {
        auto sheet = CreateSheet();
        sheet->SetCell("A1"_pos, "=(1+2)*3");
        sheet->SetCell("A2"_pos, "some");
        sheet->SetCell("B1"_pos, "=1+(2*3)");
        sheet->SetCell("B2"_pos, "text");
        sheet->PrintTexts(std::cout);
        sheet->ClearCell("A2"_pos);
        sheet->PrintValues(std::cout);
    } 
```
### Вывод
```
=(1+2)*3	=1+2*3
some	text

9	7
	text
```
