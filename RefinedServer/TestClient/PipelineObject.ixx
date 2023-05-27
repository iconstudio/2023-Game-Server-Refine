export module PipelineObject;

export extern "C++" namespace
{
	class [[nodiscard]] PipelineObject
	{
	public:
		virtual PipelineObject() noexcept = 0;
		virtual PipelineObject(const PipelineObject& other) noexcept = delete;
		virtual PipelineObject(PipelineObject&& other) noexcept = default;
		virtual PipelineObject& operator=(const PipelineObject& other) noexcept = delete;
		virtual PipelineObject& operator=(PipelineObject&& other) noexcept = default;

		virtual ~PipelineObject() noexcept
		{}

		virtual void Awake() = 0;
		virtual void Start() = 0;
		virtual void Update(const float& delta_time) = 0;
		virtual void LateUpdate(const float& delta_time) = 0;

		bool operator==(const PipelineObject& other) const noexcept = default;
	};
}
