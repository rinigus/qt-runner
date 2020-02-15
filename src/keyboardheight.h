#ifndef KEYBOARDHEIGHT_H
#define KEYBOARDHEIGHT_H

#include <QObject>

class KeyboardHeight : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int height READ height NOTIFY heightChanged)

public:
  explicit KeyboardHeight(QObject *parent = nullptr);

  int height() const { return m_height; }
  void setHeight(int h);

signals:
  void heightChanged(int height);

protected:
  int m_height;

};

#endif // KEYBOARDHEIGHT_H
