#include <iostream>
#include <fstream> 
#include <string> 

using namespace std;

template <typename T>
class Matrix{
private: 
    int lines, columns;
    T** matrix;
    void clear(){
        if (matrix != nullptr) {
            for (int i=0; i<lines; i++) {
                delete[] matrix[i];
            }
        delete[] matrix;
        }
    }
public: 
    //Конструктор по умолчанию(пустая матрица)
    Matrix() {
        lines = 0;
        columns = 0;
        matrix = nullptr;
    } 
    //Конструктор матрицы с заданными размерами
    Matrix(int k_lines, int k_columns) {
        lines = k_lines;
        columns = k_columns;
        matrix = new T* [lines]; //выделение динамической памяти под массив указателей
        for (int i=0; i<lines; i++) {
             matrix[i] = new T [columns]; //выделение динамической памяти для массива значений
        }
    }
    //Конструктор для считывания матрицы из файла
    Matrix(ifstream& in) {
        in >> lines;
        in >> columns;
        matrix = new T* [lines];
        for (int i=0; i<lines; i++) {
            matrix[i] = new T [columns];
            for (int j=0; j<columns; j++) {
                in >> matrix[i][j];
            }
        }
    }
    //Конструктор копирования
    Matrix(const Matrix & M) {
        lines = M.lines;
        columns = M.columns;
        matrix = new T* [lines];
        T** mx = matrix;
        for (int i=0; i<lines; i++) {
             matrix[i] = new T [columns];
        }
        for (int i=0; i<lines; i++){
            for (int j=0; j<columns; j++) {
                mx[i][j]=M.matrix[i][j];
            }
        }

    }
    //Деструктор
    ~Matrix() {
        clear();
    }

    //Ввод с консоли >>
    friend istream& operator>>(istream& in, Matrix& M) {
        cout << "Ввод матрицы" << endl;
        if (M.lines == 0 || M.columns == 0) {
            cout << "Введите количество строк: ";
            in >> M.lines;
            cout << "Введите количество столбцов: ";
            in >> M.columns;
        }
        M.matrix = new T* [M.lines];
        for (int i=0; i<M.lines; i++) {
            M.matrix[i] = new T [M.columns];
            for (int j=0; j<M.columns; j++) {
                cout << "Введите число: строка - " << i+1 << "; столбец - " << j+1 << ": ";
                in >> M.matrix[i][j];
            }
        }
        cout << endl;
        return in;
    }
    //Вывод в консоль <<
    friend ostream& operator<<(ostream& out, const Matrix& M) {
        for (int i=0; i<M.lines; i++) {
            for (int j=0; j<M.columns; j++) {
                out << M.matrix[i][j] << " ";
            }
            out << endl;
        }
        out << endl;
        return out;
    }
    //Ввод с файла >>
    friend ifstream& operator>>(ifstream& infile, Matrix& M) {
        infile >> M.lines;
        infile >> M.columns;
        M.matrix = new T* [M.lines];
        for (int i=0; i<M.lines; i++) {
            M.matrix[i] = new T [M.columns];
            for (int j=0; j<M.columns; j++) {
                infile >> M.matrix[i][j];
            }
        }
        return infile;
    }
    //Вывод в файл <<
    friend ofstream& operator<<(ofstream& outfile, const Matrix& M) {
        for (int i=0; i<M.lines; i++) {
            for (int j=0; j<M.columns; j++) {
                outfile << M.matrix[i][j] << " ";
            }
            outfile << endl;
        }
        return outfile;
    }

    //перегрузка операторов + и -
    Matrix operator +(const Matrix & second) {
        if (this->lines == second.lines && this->columns == second.columns) {
            cout << "Матрица сложения:" << endl;
            Matrix M(lines,columns);
            for (int i=0; i<lines; i++) {
                for (int j=0; j<columns; j++) {
                    M.matrix[i][j] = this->matrix[i][j] + second.matrix[i][j];
                }
            }
            return M;
        } 
        throw "Матрицы разного порядка, сложить нельзя!"; //Оператор throw генерирует исключение, которое представлено временным объектом типа PlusError
    }
    Matrix operator -(const Matrix & second) {
        if (this->lines == second.lines && this->columns == second.columns) {
            cout << "Матрица вычитания:" << endl;
            Matrix M(lines,columns);
            for (int i=0; i<lines; i++) {
                for (int j=0; j<columns; j++) {
                    M.matrix[i][j] = this->matrix[i][j] - second.matrix[i][j];
                }
            }
            return M;
        } 
        throw "Матрицы разного порядка, вычесть нельзя!";
    }

    //перегрузка оператора * для умножения матриц и умножения матрицы на скаляр
    Matrix operator *(const Matrix & second) {
        if (this->columns == second.lines) {
            cout << "Матрица произведения:" << endl;
            Matrix M(lines,second.columns);
            for (int i=0; i<lines; i++) {
                for (int j=0; j<second.columns; j++) {
                    M.matrix[i][j] = 0;
                    for (int k=0; k<lines; k++) {
                        M.matrix[i][j] += this->matrix[i][k] * second.matrix[k][j];
                    }
                }
            }
            return M;
        }
        throw "Кол-во столбцов 1-й матрицы не равно кол-ву строк 2-й матрицы, умножить нельзя!";
    } 
    Matrix operator *(T scalar) {
        cout << "Матрица, умноженная на скаляр:" << endl;
        Matrix M(lines,columns);
        for (int i=0; i<lines; i++) {
            for (int j=0; j<columns; j++) {
                M.matrix[i][j] = this->matrix[i][j] * scalar;
            }
        }
        return M;
    }



    /*set-функции инициализируют элементы данных
    void setMatrix(int n_lines, int n_columns) {
        for (int i=0; i<lines; i++) {
            for (int j=0; j<columns; j++) {
                double number;
                cout << "Введите число: строка - " << i+1 << "; столбец - " << j+1 << ":";
                cin >> number;
                matrix[i][j] = number;
            }
        }
    }

    //get-функции позволяют посмотреть значения элементов данных
    void getMatrix(const char* name) {
        cout << "Матрица " << name << endl;
        for (int i=0; i<lines; i++) {
            for (int j=0; j<columns; j++) {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    
    
    //перегрузка операторов == и != для сравнения матриц
    bool operator ==(const Matrix & second){
        if (this->lines == second.lines && this->columns == second.columns) { //если размерности матриц совпадают
            for (int i=0; i<lines; i++) {
                for (int j=0; j<columns; j++) {
                    if (this->matrix[i][j] != second.matrix[i][j]) {//проверяем элементы на равенство
                        cout << "Матрицы не равны" << endl;;
                        return false;//при нахождении первого несовпадения false
                    }
                } 
            }
            cout <<  "Матрицы равны" << endl;
            return true; //все совпало true
        } else {
            cout << "Матрицы не равны" << endl;
            return false;//при сравнении матриц их размерность не совпала
        }
    }
    bool operator !=(const Matrix & second){
        return !(*this == second); //разыменовали this, при проверке на неравенство ссылаемся на равенство
    }
    //перегрузка операторов == и != для сравнения матрицы и скаляра 
    bool operator ==(double scalar) {
        for (int i=0; i<lines; i++) {
            for(int j=0; j<columns; j++) {
                if (i!=j && matrix[i][j] != 0) {//если элемент вне главной диагонали не ноль false
                    cout << "Матрица не равна скаляру" << endl;
                    return false;
                } 
                if (i==j && matrix[i][j] != scalar) {//если  элемент на главной диагонали не равен скаляру false
                    cout << "Матрица не равна скаляру" << endl;
                    return false;
                }
            }
        }
        cout << "Матрица равна скаляру" << endl;
        return true;//равенства выполняются true
    }

    bool operator !=(double scalar) {
        return !(*this == scalar);
    }
    //функция для сложения строк или столбцов
    //если thing = 0 - строка, если thing = 1 - столбец
    void plus(int thing, int one, int two, double L) {
        if (thing == 0) {
            for (int i=0; i<columns; i++) {
                matrix[one-1][i] += matrix[two-1][i] * L;
            }
        } else {
            for (int i=0; i<lines; i++) {
                matrix[i][one-1] += matrix[i][two-1] * L;
            }
        }
    }    
    //функция для вычитания строк или столбцов
    void minus(int thing, int one, int two, double L) {
        if (thing == 0) {
            for (int i=0; i<columns; i++) {
                matrix[one-1][i] -= matrix[two-1][i] * L;
            }
        } else {
            for (int i=0; i<lines; i++) {
                matrix[i][one-1] -= matrix[i][two-1] * L;
            }
        }
    }
    //функция для преобразования отдельной строки или столбца
    void multiply(int thing, int one, double L) {
        if (thing == 0) {
            for (int i=0; i<columns; i++) {
                matrix[one-1][i] *= L;
            }
        } else {
            for (int i=0; i<lines; i++) {
                matrix[i][one-1] *= L;
            }
        }
    }
    //функция для перестановки строки или столбца
    void change(int thing, int one, int two) {
       int temp;
       if (thing == 0) {
            for (int i=0; i<columns; i++) {
                temp = matrix[one-1][i];
                matrix[one-1][i] = matrix[two-1][i];
                matrix[two-1][i] = temp;
            }
        } else {
            for (int i=0; i<lines; i++) {
                temp = matrix[i][one-1];
                matrix[i][one-1] = matrix[i][two-1];
                matrix[i][two-1] = temp;
            }
        } 
    }
    
    //функция для нахождения детерминанта
    double determinant() {
        double det=0;
        int one;
        double ** M = this->matrix;
        if (this->lines != this->columns) {//матрица не квадратная, определителя нет
            cout <<  "Невозможно найти определитель!" << endl;
            return 0;
        } else {
            if (lines == 1) { //если порядок матрицы 1 - выводим определитель
                return matrix[0][0];
            } else if (lines == 2) { //порядок матрицы 2 - выводим определитель по формуле
                return (matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0]);
            } else {//если порядок больше - будем искать через матрицы все меньшего порядка
                det = 0;
                for (int i=0; i<lines; i++) {
                    Matrix temp(lines-1,columns-1);
                    for (int j=1; j<lines; j++) {
                        int t = 0;
                        for (int k=0; k<lines; k++) {
                            if (k == i) { //если столбец равен тому, который должен быть вычеркнут, пропускаем его
                                continue;
                            }
                            temp.matrix[j-1][t] = matrix[j][k];
                            t++;
                        }
                    }
                    //знак каждого слагаемого для определителя
                    //i+1 - столбец, +1 - первая строка
                    if ((i+1+1)%2==0) {
                        one = 1;
                    } else {
                        one = -1;
                    }
                    //формула - det += pow(-1,i+1+1)*matrix[0][i]*temp.determinant()
                    //matrix[0][i] - элемент из первой строки
                    det += one*matrix[0][i]*temp.determinant();
                }
            }
        }
        return det;
    }
    //функция для нахождения дополнений
    double addition(int stroka, int stolb) {
        Matrix temp(lines-1,columns-1); //матрица на порядок меньше исходной
        int m_l,m_c; //строки и столбцы исходной матрицы
        m_l=0;
        for (int t_l=0; t_l<lines-1; t_l++) {
            if (t_l==stroka-1) {
                m_l=1;
            }
            m_c=0;
            for (int t_c=0; t_c<lines-1; t_c++) {
                if (t_c == stolb-1) {
                    m_c = 1;
                }
                temp.matrix[t_l][t_c] = matrix[t_l+m_l][t_c+m_c];//заполняем элементами, кроме вычеркнутых строки и столбца
            }
        }
        //знак каждого дополнения
        int one;
        if ((stroka+stolb)%2==0) {
            one = 1;
        } else {
            one = -1;
        }
        return one*temp.determinant();
    }

    //функция для нахождения присоединенной матрицы - A*
    Matrix join() {
        double ** M = this->matrix;
        Matrix temp(lines,columns);//передаем во временную матрицу исходную
        for (int i=0; i<lines; i++) {
                for (int j=0; j<columns; j++){
                    temp.matrix[i][j] = matrix[i][j]; 
                }
        }
        Matrix T(lines,columns);//создаем конечную матрицу
        if (lines != columns) {
            cout << "Присоединенной матрицы нет! Обратите внимание, что исходная не квадратная: ";
            return temp;
        } else {
            for (int i=0; i<lines; i++) {
                for (int j=0; j<columns; j++){
                    T.matrix[i][j] = temp.addition(i+1,j+1); //заполняем элементы конечной матрицы дополнениями, найденными из исходной
                }
            }
        } 
        return T;
    }
    //функция для транспонирования матрицы
    void transposed() {
        Matrix temp(lines,columns);
        for (int i=0; i<lines; i++) {
            for (int j=0; j<lines; j++) {
                temp.matrix[i][j] = matrix[i][j];
            }
        }
        for (int i=0; i<lines; i++) {
            for (int j=0; j<lines; j++) {
                matrix[i][j] = temp.matrix[j][i];
            }
        }
    }
    //перегрузка оператра ! для вычисления обраной матрицы
    Matrix operator !() {
        Matrix temp(lines,columns);//передаем во временную матрицу исходную
        for (int i=0; i<lines; i++) {
            for (int j=0; j<lines; j++) {
                temp.matrix[i][j] = matrix[i][j];
            }
        }
        double det = temp.determinant();
        if (det == 0) {
            cout << "Обратной матрицы не существует! ";
            Matrix zero(lines,columns);
            for (int i=0; i<lines; i++) {
                for (int j=0; j<lines; j++) {
                    zero.matrix[i][j] = 0;
                }
            }
            return zero;
        } else {
            Matrix temp_2 = temp.join();//во вторую временную матрицу передаем присоединенную для исходной
            temp_2.transposed();
            Matrix temp_3 = temp_2 * (1/det);//третья временная матрица - обратная
            cout << "Обратная матрица для заданной ";
            return temp_3;
        }
    }
    Matrix operator =(const Matrix & second) {
        clear();
        //4x4 5x5 
        lines = second.lines;
        columns=second.columns;
        matrix = new double * [lines];
        for (int i=0; i<lines; i++) {
            matrix[i] = new double [columns];
            for (int j=0; j<columns; j++) {
                matrix[i][j] = second.matrix[i][j];
            }
        }
        return *this;
    }
*/
};

int main() {
    //1-2  
    Matrix<int> A_1;
    cin >> A_1;
    cout << "Матрица ввода с размером А_1:" << endl << A_1;

    Matrix<int> A_2(3,4);
    cin >> A_2;
    cout << "Матрица ввода без размеров А_2:" << endl << A_2; 

    ifstream file_B("matrix_B.txt");
    Matrix<int> B(file_B);  
    file_B.close();
    cout << "Матрица из файла B:" << endl << B;

    ifstream file_C("matrix_C.txt");
    Matrix<double> C;
    file_C >> C;
    ofstream write("write_matrix.txt");
    write << "Матрица из одного файла в другой С:" << endl << C;
    file_C.close();
    write.close();

    //3
    try {
        Matrix<double> R_1;
        cin >> R_1;
        Matrix<double> R_2;
        cin >> R_2;
        try {
            cout << R_1 + R_2;;
        } catch (const char* error_message){//через параметр в блоке catch мы можем получить то сообщение, которое передается оператору throw
            cout << error_message << endl;
        }
        try {
            cout << R_1 - R_2;
        } catch (const char* error_message){
            cout << error_message << endl;
        }   
    } catch (...) {//является последним блоком catch в цепочке обработки исключений и предназначен для обработки общих ситуаций ошибок или случаев, которые не предусмотрены более конкретными блоками catch
        cout << endl;  
    }

    //4
    try {
        Matrix<int> P_1;
        cin >> P_1;
        Matrix<int> P_2;
        cin >> P_2; 
        cout << P_1 * P_2;
    } catch(const char* error_message) {
        cout << error_message << endl;
    }
    Matrix<double> S;
    cin >> S;
    double s;
    cout << "Введите скаляр: ";
    cin >> s;
    cout << S*s;


    /*
    
    Matrix C = A * B;
    C.getMatrix("C");

    double s;
    cout << "Введите скаляр: ";
    cin >> s;
    Matrix D = A * s;
    D.getMatrix("D - матрица, умноженная на скаляр: ");

   Matrix E = A + B;
    E.getMatrix("E");
    Matrix F = A - B;
    F.getMatrix("F");

    bool equal = A==B;
    bool not_equal = A!=B;

    double r;
    cout << "Введите скаляр: ";
    cin >> r;
    bool equal_scalar = A==r;
    bool not_equal_scalar = A!=r;
    

    cout << "Определитель матрицы A: " << A.determinant() << endl;

    Matrix G = !A;
    G.getMatrix("A^(-1) обратная для A: ");*/

}