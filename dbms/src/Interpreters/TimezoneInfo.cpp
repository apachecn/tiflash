#include <Interpreters/TimezoneInfo.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <tipb/select.pb.h>
#pragma GCC diagnostic pop

namespace DB
{
void TimezoneInfo::resetByTimezoneName(const String & name)
{
    is_name_based = true;
    timezone_offset = 0;
    timezone = &DateLUT::instance(name);
    timezone_name = timezone->getTimeZone();
    is_utc_timezone = timezone_name == "UTC";
}

void TimezoneInfo::resetByTimezoneOffset(Int64 offset)
{
    is_name_based = false;
    timezone_offset = offset;
    timezone = &DateLUT::instance("UTC");
    timezone_name = "";
    is_utc_timezone = timezone_offset == 0;
}

void TimezoneInfo::resetByDAGRequest(const tipb::DAGRequest & rqst)
{
    if (rqst.has_time_zone_name() && !rqst.time_zone_name().empty())
    {
        // dag request use name based timezone info
        resetByTimezoneName(rqst.time_zone_name());
    }
    else if (rqst.has_time_zone_offset())
    {
        // dag request use offset based timezone info
        resetByTimezoneOffset(rqst.time_zone_offset());
    }
    else
    {
        // dag request does not have timezone info
        resetByTimezoneOffset(0);
    }
}

} // namespace DB