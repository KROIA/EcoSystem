#pragma once

#include "EcoSystem_base.h"
#include <AppSettings.h>

namespace EcoSystem
{
	namespace SaveSystem
	{
		class ApplicationSettings;

		class ECO_SYSTEM_EXPORT SimulationSettings : public AppSettings::SettingsGroup
		{
			friend ApplicationSettings;
			SimulationSettings();
		public:
			

		};

		class ECO_SYSTEM_EXPORT AssetSettings : public AppSettings::SettingsGroup
		{
			friend ApplicationSettings;
			AssetSettings();
		public:

			const std::string getAssetPath() const { return m_assetPath.getValue().toString().toStdString(); }
			const std::string getTexturePath() const { return (m_assetPath.getValue().toString() + "/" + m_relativeTexturePath.getValue().toString()).toStdString(); }


			AppSettings::Setting m_assetPath = AppSettings::Setting("AssetPath", "../Assets");
			AppSettings::Setting m_relativeTexturePath = AppSettings::Setting("RelativeTexturePath", "Textures");
		};
		

		class ECO_SYSTEM_EXPORT ApplicationSettings : public AppSettings::ApplicationSettings
		{
			ApplicationSettings();
			
		public:
			static ApplicationSettings& getInstance();

			static SimulationSettings& getSimulationSettings() { return getInstance().m_simulationSettings; }
			static AssetSettings& getAssetSettings() { return getInstance().m_assetSettings; }

		private:

			SimulationSettings m_simulationSettings;
			AssetSettings m_assetSettings;
		};
	}
}