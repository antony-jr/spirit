#ifndef ACTIVE_WINDOW_TRACKER_ENUMS_HPP_INCLUDED
#define ACTIVE_WINDOW_TRACKER_ENUMS_HPP_INCLUDED

struct ActiveWindowTrackerEnums { 
   struct Error {
      enum : short {
	   NoError = 0,
	   UnSupportedPlatform
      };
   };
};

#endif // ACTIVE_WINDOW_TRACKER_ENUMS_HPP_INCLUDED
