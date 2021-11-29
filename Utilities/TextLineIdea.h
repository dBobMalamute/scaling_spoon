#ifndef TEXTLINEIDEA_H
#define TEXTLINEIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"

struct TextLineAttributes : public IdeaAttributes
{
    TextLineAttributes();
};

class TextLineIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(bool bold READ bold WRITE setBold NOTIFY boldChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

public:
    explicit TextLineIdea();
    const IdeaAttributes attributes() override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    QString text() const;   
    bool bold() const;   
    int fontSize() const;

public slots:
    void setText(QString text);
    void setBold(bool bold);    
    void setFontSize(int fontSize);

signals:
    void textChanged();  
    void boldChanged(bool bold);

    void fontSizeChanged(int fontSize);

private:
    QString m_text;
    bool m_bold;
    int m_fontSize;
};

#endif // TEXTLINEIDEA_H
