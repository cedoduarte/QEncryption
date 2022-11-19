/**
 * @author Carlos Enrique Duarte Ortiz (carlosduarte.1@hotmail.com)
 * @date November 18th 2022
 * @version 1.0
 */

#ifndef QENCRYPTION_H
#define QENCRYPTION_H

#include <vector>
#include <QString>

/** input for encryption */
class QEncryptionInput
{
public:
    /** constructor */
    explicit QEncryptionInput();

    /** destructor */
    virtual ~QEncryptionInput();

    /** sets input for encryption/decryption */
    void setInput(const QString &input);

    /** sets the encryption/decryption password */
    void setPassword(const QString &password);

    /** sets the encryption matrix */
    void setMatrix(const std::vector<QString> &matrix);

    /** gets the input */
    QString input() const;

    /** gets the password */
    QString password() const;

    /** gets the encryption matrix */
    const std::vector<QString> &matrix() const;
private:
    QString m_input; //!< input
    QString m_password; //!< password
    std::vector<QString> m_matrix; //!< encryption matrix
};

/** encryption functions */
class QEncryption
{
public:
    /** encrypts */
    static QString encrypt(const QEncryptionInput &input);
    static QString encrypt(const QString &input, const QString &password, const std::vector<QString> &matrix);
    static QString encrypt(const QString &input, const QString &password, const QString &matrixFilepath);

    /** decrypts */
    static QString decrypt(const QEncryptionInput &input);
    static QString decrypt(const QString &input, const QString &password, const std::vector<QString> &matrix);
    static QString decrypt(const QString &input, const QString &password, const QString &matrixFilepath);

    /** gets random encryption matrix */
    static std::vector<QString> generateRandomMatrix();

    /** saves random encryption matrix */
    static bool saveRandomMatrix(const QString &outputFilepath);

    /** saves matrix */
    static bool saveMatrix(const std::vector<QString> &matrix, const QString &outputFilepath);

    /** loads encryption matrix from file */
    static std::vector<QString> loadMatrix(const QString &inputFilepath);
private:
    static void setCycles(int cycles = 1000);
    explicit QEncryption();
    virtual ~QEncryption();
    void encryptData();
    void decryptData();
    void setData(const QString &data);
    void setPassword(const QString &password);
    QString getData() const;
    QString getEncryptedData() const;
    QString getDecryptedData() const;
    QString getPassword() const;
    void loadMatrix();
    void setMatrix(const std::vector<QString> &matrix);
    const std::vector<QString> &getMatrix() const;
    void initializeMatrix();
    void scrambleMatrix(int cycles);
    void swapMatrixCells(int aIndex, int bIndex);
    void encryptAndSave(const QString &input);
    void decryptFunction(const QStringList &L, QString &input);

    QString m_data;
    QString m_password;
    QString m_encryptedData;
    QString m_decryptedData;
    std::vector<QString> m_matrix;
    static int s_cycles;
};

#endif // QENCRYPTION_H
