#ifndef custom_comp_hpp
#define custom_comp_hpp

#include <string>

namespace xsm::comp{
  
  struct StartsWithK {};
  
  struct CompK{
    using is_transparent = void;
    bool operator()(std::string lhs, std::string rhs) const {
      return lhs.compare(rhs) < 0;
    }
    bool operator()(std::string lhs, StartsWithK rhs) const {
      return tolower(lhs[0]) < 'k';
    }
    bool operator()(StartsWithK lhs, std::string rhs) const {
      return 'k' < tolower(rhs[0]);
    }
  };

  struct CompInv{
    using is_transparent = void;
    bool operator()(std::string lhs, std::string rhs) const {
      return lhs.compare(rhs) > 0; // Inverted order
    }
  };
}

#endif
