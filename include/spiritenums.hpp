#ifndef SPIRIT_ENUMS_HPP_INCLUDED
#define SPIRIT_ENUMS_HPP_INCLUDED

struct SpiritEnums {
   struct ActiveWindowTracker {	
      struct Error {
         enum : short {
	      NoError = 0,
	      UnSupportedPlatform
         };
      };
   };

   struct Spirit {
       struct Error {
          enum : short {
	      NoError = 0,
	      InvalidSpiritFile
          };
       };
   };
};

#endif // SPIRIT_ENUMS_HPP_INCLUDED
