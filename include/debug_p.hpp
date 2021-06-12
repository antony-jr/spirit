#ifndef SPIRIT_DEBUG_HPP_PRIVATE_INCLUDED
#define SPIRIT_DEBUG_HPP_PRIVATE_INCLUDED
#ifdef ENABLE_DEBUGGING
#include <QDebug>
#define LOG_START qDebug() <<
#define LOG_REDIR <<
#define LOG_END   ;
#else
#define LOG_START if(0){(void)
#define LOG_REDIR ;(void)
#define LOG_END   ;}
#endif // ENABLE_DEBUGGING
#endif // SPIRIT_DEBUG_HPP_PRIVATE_INCLUDED

