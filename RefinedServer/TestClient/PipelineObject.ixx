export module System.PipelineObject;
import Utility;

export extern "C++" namespace sys
{
	template<typename T>
	concept PipelineModel = requires(T object)
	{
		object.Awake();
		object.Start();
		object.Update(0.0f);
		object.LateUpdate(0.0f);
	};

	template<typename T>
	concept PipelineModel = requires(T object)
	{
		object.Awake();
		object.Start();
		object.Update(0.0f);
		object.LateUpdate(0.0f);
	};

	template<typename O>
	class [[nodiscard]] PipelineObject
	{
	public:
		constexpr PipelineObject() noexcept = default;
		constexpr ~PipelineObject() noexcept = default;

		void Awake() = 0;
		void Start() = 0;
		void Update(const float& delta_time) = 0;
		void LateUpdate(const float& delta_time) = 0;

		[[nodiscard]]
		constexpr bool operator==(const PipelineObject& other) const
			noexcept(noexcept(util::declval<const O>() == util::declval<const O>()))
		{
			return *(Cast()) == *(other.Cast());
		}

		PipelineObject(const PipelineObject& other) noexcept = delete;
		constexpr PipelineObject(PipelineObject&& other) noexcept = default;
		PipelineObject& operator=(const PipelineObject& other) noexcept = delete;
		constexpr PipelineObject& operator=(PipelineObject&& other) noexcept = default;

	protected:
		[[nodiscard]]
		constexpr O* Cast() noexcept
		{
			return static_cast<O*>(this);
		}

		[[nodiscard]]
		constexpr const O* Cast() const noexcept
		{
			return static_cast<const O*>(this);
		}

		friend O;
	};
}
