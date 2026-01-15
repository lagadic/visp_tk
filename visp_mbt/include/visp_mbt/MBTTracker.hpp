#ifndef MBT_TRACKER_HPP
#define MBT_TRACKER_HPP

#include <visp_tracker_common/BaseMultiModalTracker.hpp>

namespace visp_mbt
{
class MBTTracker : public visp_tracker_common::BaseMultiModalTracker
{
public:
  MBTTracker(const std::string &name);
  virtual ~MBTTracker() = default;
protected:
  /** @name  Initialization */
  //@{

  /**
   * @brief Initilize the tracker used by the servoing node.
   *
   * @return true The initialization went well
   * @return false A problem occured
   */
  virtual bool init_tracker() override;

  /**
   * @brief Initialize the m_info_strings vector with constant strings to
   * give the user some info.
   */
  virtual void init_info_strings() override;

  /**
   * @brief Check the tracker settings in order to know if the depth
   * is actually required and set m_depth_is_required accordingly.
   */
  virtual void check_requires_depth() override;

  //@}

};
}

#endif
