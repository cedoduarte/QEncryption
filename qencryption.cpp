/**
 * @author Carlos Enrique Duarte Ortiz (carlosduarte.1@hotmail.com)
 * @date November 18th 2022
 * @version 1.0
 */

#include "qencryption.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QRandomGenerator>

QEncryptionInput::QEncryptionInput()
{
    // nothing to do here
}

QEncryptionInput::~QEncryptionInput()
{
    // nothing to do here
}

void QEncryptionInput::setInput(const QString &input)
{
    m_input = input;
}

void QEncryptionInput::setPassword(const QString &password)
{
    m_password = password;
}

void QEncryptionInput::setMatrix(const std::vector<QString> &matrix)
{
    m_matrix = matrix;
}

QString QEncryptionInput::input() const
{
    return m_input;
}

QString QEncryptionInput::password() const
{
    return m_password;
}

const std::vector<QString> &QEncryptionInput::matrix() const
{
    return m_matrix;
}

QString QEncryption::encrypt(const QEncryptionInput &input)
{
    return encrypt(input.input(), input.password(), input.matrix());
}

QString QEncryption::decrypt(const QEncryptionInput &input)
{
    return decrypt(input.input(), input.password(), input.matrix());
}

QString QEncryption::encrypt(const QString &input, const QString &password, const std::vector<QString> &matrix)
{
    QEncryption e;
    e.setMatrix(matrix);
    e.setData(input);
    e.setPassword(password);
    e.encryptData();
    return e.getEncryptedData();
}

QString QEncryption::encrypt(const QString &input, const QString &password, const QString &matrixFilepath)
{
    QEncryption e;
    e.setMatrix(loadMatrix(matrixFilepath));
    e.setData(input);
    e.setPassword(password);
    e.encryptData();
    return e.getEncryptedData();
}

QString QEncryption::decrypt(const QString &input, const QString &password, const std::vector<QString> &matrix)
{
    QEncryption e;
    e.setMatrix(matrix);
    e.m_encryptedData = input;
    e.setPassword(password);
    e.decryptData();
    return e.getDecryptedData();
}

QString QEncryption::decrypt(const QString &input, const QString &password, const QString &matrixFilepath)
{
    QEncryption e;
    e.setMatrix(loadMatrix(matrixFilepath));
    e.m_encryptedData = input;
    e.setPassword(password);
    e.decryptData();
    return e.getDecryptedData();
}

std::vector<QString> QEncryption::generateRandomMatrix()
{
    QEncryption e;
    e.loadMatrix();
    return e.getMatrix();
}

bool QEncryption::saveRandomMatrix(const QString &outputFilepath)
{
    return saveMatrix(generateRandomMatrix(), outputFilepath);
}

bool QEncryption::saveMatrix(const std::vector<QString> &matrix, const QString &outputFilepath)
{
    QFile F(outputFilepath);
    if (F.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&F);
        stream.setGenerateByteOrderMark(true);
        for (int i = 0; i < int(matrix.size()); ++i)
        {
            stream << matrix[i];
        }
        F.flush();
        F.close();
        return true;
    }
    return false;
}

std::vector<QString> QEncryption::loadMatrix(const QString &inputFilepath)
{
    std::vector<QString> matrix;
    QFile F(inputFilepath);
    if (F.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&F);
        QString content = stream.readAll();
        F.close();
        for (int i = 0; i < int(content.size()); ++i)
        {
            matrix.push_back(content[i]);
        }
    }
    return matrix;
}

int QEncryption::s_cycles = 1000;

void QEncryption::setCycles(int cycles)
{
    s_cycles = cycles;
}

QEncryption::QEncryption()
{
    // nothing to do here
}

QEncryption::~QEncryption()
{
    // nothing to do here
}

void QEncryption::encryptData()
{
    if (m_password.isEmpty())
    {
        return;
    }
    if (m_data.isEmpty())
    {
        return;
    }
    m_encryptedData.clear();
    encryptAndSave(m_password);
    m_encryptedData.resize(m_encryptedData.size() - 1);
    m_encryptedData.append("#");
    encryptAndSave(m_data);
    m_encryptedData.resize(m_encryptedData.size() - 1);
}

void QEncryption::decryptData()
{
    if (m_password.isEmpty())
    {
        return;
    }
    if (m_encryptedData.isEmpty())
    {
        return;
    }
    m_decryptedData.clear();
    QStringList L = m_encryptedData.split("#");
    QString passphrase = L[0];
    QString encrypted = L[1];
    QString decryptedPassphrase;
    L = passphrase.split(" ");
    decryptFunction(L, decryptedPassphrase);
    if (m_password == decryptedPassphrase)
    {
        L = encrypted.split(" ");
        decryptFunction(L, m_decryptedData);
    }
}

void QEncryption::setData(const QString &data)
{
    m_data = data;
}

void QEncryption::setPassword(const QString &password)
{
    m_password = password;
}

QString QEncryption::getData() const
{
    return m_data;
}

QString QEncryption::getEncryptedData() const
{
    return m_encryptedData;
}

QString QEncryption::getDecryptedData() const
{
    return m_decryptedData;
}

QString QEncryption::getPassword() const
{
    return m_password;
}

void QEncryption::loadMatrix()
{
    initializeMatrix();
    scrambleMatrix(s_cycles);
}

void QEncryption::setMatrix(const std::vector<QString> &matrix)
{
    m_matrix = matrix;
}

const std::vector<QString> &QEncryption::getMatrix() const
{
    return m_matrix;
}

void QEncryption::initializeMatrix()
{
    m_matrix.clear();
    for (int i = 0; i <= 9; ++i)
    {
        m_matrix.push_back(QString::number(i));
    }
    for (char c = 'a'; c <= 'z'; ++c)
    {
        m_matrix.push_back(QString(QChar(c)));
        m_matrix.push_back(QString(QChar(c).toUpper()));
    }
    static const QString specialSymbols = "<>!\"·$%&/()=?¿|@#~€¬'¡ºª\\`^[+*]´¨{çÇ},;.:-_ ";
    for (const QChar &c : specialSymbols)
    {
        m_matrix.push_back(QString(c));
    }
}

void QEncryption::scrambleMatrix(int cycles)
{
    const int N = m_matrix.size();
    for (int i = 0; i < cycles; ++i)
    {
        const int aIndex = QRandomGenerator::global()->bounded(0, N);
        const int bIndex = QRandomGenerator::global()->bounded(0, N);
        swapMatrixCells(aIndex, bIndex);
    }
}

void QEncryption::swapMatrixCells(int aIndex, int bIndex)
{
    QString value = m_matrix[aIndex];
    m_matrix[aIndex] = m_matrix[bIndex];
    m_matrix[bIndex] = value;
}

void QEncryption::encryptAndSave(const QString &input)
{
    for (int i = 0; i < int(input.size()); ++i)
    {
        for (int m = 0; m < int(m_matrix.size()); ++m)
        {
            if (input[i] == m_matrix[m])
            {
                m_encryptedData.append(QString::number(m) + " ");
            }
        }
    }
}

void QEncryption::decryptFunction(const QStringList &L, QString &input)
{
    for (int i = 0; i < int(L.size()); ++i)
    {
        for (int m = 0; m < int(m_matrix.size()); ++m)
        {
            if (m == L[i].toInt())
            {
                input.append(m_matrix[m]);
            }
        }
    }
}
