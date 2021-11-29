#ifndef TEXTAREAIDEA_H
#define TEXTAREAIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"

struct TextAreaAttributes : public IdeaAttributes
{
    TextAreaAttributes();
};

class TextAreaIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(bool bold READ bold WRITE setBold NOTIFY boldChanged)

public:
    explicit TextAreaIdea();
    const IdeaAttributes attributes() override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    QString text() const;  
    int fontSize() const; 
    bool bold() const;

public slots:
    void setText(QString text);
    void setFontSize(int fontSize);  
    void setBold(bool bold);

signals:
    void textChanged();   
    void fontSizeChanged(int fontSize);

    void boldChanged(bool bold);

private:
    QString m_text;
    int m_fontSize;
    bool m_bold;
};

#endif // TEXTAREAIDEA_H
