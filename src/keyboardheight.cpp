#include "keyboardheight.h"

KeyboardHeight::KeyboardHeight(QObject *parent) : QObject(parent), m_height(0)
{
}

void KeyboardHeight::setHeight(int h)
{
  m_height = h;
  emit heightChanged(m_height);
}
