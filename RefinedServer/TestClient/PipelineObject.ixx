export module System.PipelineObject;
import Utility;
import Utility.Constraints;

export extern "C++" namespace sys
{
	struct PipelineModel
	{
		constexpr PipelineModel() noexcept = default;
		virtual constexpr ~PipelineModel() noexcept = default;

		virtual void Awake() {}
		virtual void Start() noexcept {}
		virtual void Update(const float delta_time) {}
		virtual void LateUpdate(const float delta_time) {}
		virtual void Destroy() {}
	};

	template<typename O>
	concept pipelines = requires(O object)
	{
		object.Awake();
		object.Start();
		object.Update(0.0f);
		object.LateUpdate(0.0f);
		object.Destroy();
	};

	/// <summary>
	/// Will not be used until deducing this is not implemented.
	/// </summary>
	/// <typeparam name="O"></typeparam>
	template<util::classes O>
	class [[nodiscard]] PipelineObject
	{
		static_assert(util::always_false<O>, "Don't use it for now.");

	public:
		constexpr PipelineObject() noexcept = default;
		constexpr ~PipelineObject() noexcept = default;

		void Awake()
			noexcept(noexcept(util::declval<O>().Awake()))
		{
			Cast()->Awake();
		}

		void Start()
			noexcept(noexcept(util::declval<O>().Start()))
		{
			Cast()->Start();
		}

		void Update(float delta_time)
			noexcept(noexcept(util::declval<O>().Update(util::declval<float>())))
		{
			Cast()->Update(delta_time);
		}

		void LateUpdate(float delta_time)
			noexcept(noexcept(util::declval<O>().LateUpdate(util::declval<float>())))
		{
			Cast()->LateUpdate(delta_time);
		}

		void Destroy()
			noexcept(noexcept(util::declval<O>().Destroy()))
		{
			Cast()->Destroy();
		}

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
