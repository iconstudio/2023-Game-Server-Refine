export module Utility.Time;
export import <chrono>;

export namespace util
{
	using ::std::get_time;

	using ::std::operator<<;
	using ::std::operator==;
	using ::std::operator<=>;

	using std::chrono::system_clock;
}
