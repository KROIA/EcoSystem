#include "SaveSystem/ApplicationSettings.h"

namespace EcoSystem
{
	namespace SaveSystem
	{
		SimulationSettings::SimulationSettings()
			: SettingsGroup("Simulation")
		{
			
		}

		AssetSettings::AssetSettings()
			: SettingsGroup("Assets")
		{
			addSetting(m_assetPath);
			addSetting(m_relativeTexturePath);
		}


		
		ApplicationSettings::ApplicationSettings()
			: AppSettings::ApplicationSettings("EcoSystem")
		{
			setPath("settings");

			addGroup(m_simulationSettings);
			addGroup(m_assetSettings);
		}
		ApplicationSettings& ApplicationSettings::getInstance()
		{
			static ApplicationSettings instance;
			return instance;
		}
	}
}