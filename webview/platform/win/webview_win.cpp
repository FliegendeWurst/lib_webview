// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "webview/platform/win/webview_win.h"

#include "base/random.h"
#include "base/platform/base_platform_info.h"
#include "webview/platform/win/webview_windows_edge_chromium.h"
#include "webview/platform/win/webview_windows_edge_html.h"

namespace Webview {
namespace {

[[nodiscard]] bool SystemTooOld() {
	return true;// !Platform::IsWindows8Point1OrGreater();
}

} // namespace

Available Availability() {
	if (SystemTooOld()) {
		return Available{
			.error = Available::Error::OldWindows,
			.details = "Please update your system to Windows 8.1 or later.",
		};
	} else if (EdgeChromium::Supported() || EdgeHtml::Supported()) {
		return Available{};
	}
	return Available{
		.error = Available::Error::NoWebview2,
		.details = "Please install Microsoft Edge Webview2 Runtime.",
	};
}

bool SupportsEmbedAfterCreate() {
	return !SystemTooOld()
		&& !EdgeChromium::Supported()
		&& EdgeHtml::Supported();
}

bool NavigateToDataSupported() {
	return !SystemTooOld() && EdgeChromium::Supported();
}

bool SeparateStorageIdSupported() {
	return !SystemTooOld() && EdgeChromium::Supported();
}

std::unique_ptr<Interface> CreateInstance(Config config) {
	if (SystemTooOld()) {
		return nullptr;
	} else if (auto result = EdgeChromium::CreateInstance(config)) {
		return result;
	}
	return EdgeHtml::CreateInstance(config);
}

std::string GenerateStorageToken() {
	constexpr auto kSize = 16;
	auto result = std::string(kSize, ' ');
	base::RandomFill(result.data(), result.size());
	return result;
}

void ClearStorageDataByToken(const std::string &token) {
}

} // namespace Webview
