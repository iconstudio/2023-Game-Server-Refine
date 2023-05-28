export module System.PipelineObject;

export extern "C++" namespace sys
{
	template<typename T>
	concept PseudoPipelineObjs = requires(T object)
	{
		object.Awake();
		object.Start();
		object.Update(0.0f);
		object.LateUpdate(0.0f);
	};

	class [[nodiscard]] PipelineObject
	{
	public:
		constexpr PipelineObject() noexcept
		{}

		virtual constexpr ~PipelineObject() noexcept
		{}

		virtual void Awake() = 0;
		virtual void Start() = 0;
		virtual void Update(const float& delta_time) = 0;
		virtual void LateUpdate(const float& delta_time) = 0;

		constexpr bool operator==(const PipelineObject& other) const noexcept = default;

		PipelineObject(const PipelineObject& other) noexcept = delete;
		constexpr PipelineObject(PipelineObject&& other) noexcept = default;
		PipelineObject& operator=(const PipelineObject& other) noexcept = delete;
		virtual constexpr PipelineObject& operator=(PipelineObject&& other) noexcept = default;
	};
}
