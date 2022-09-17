#ifndef GLOBALCACHE_H
#define GLOBALCACHE_H

#include <QList>

inline QList<float> dlTimeList;
inline QList<float> dlPressureList;
inline int iResolution;
inline unsigned int iPointsTotal;
inline unsigned int iPointsOut;
inline QString sFilePath = "";
inline double dMinVal = 0.0;
inline double dMaxVal = 0.0;
inline QChar cDelimiter = ';';

// Preferences
inline bool bAnimationsOn;

#endif // GLOBALCACHE_H
