Name:       qt-runner

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    Qt runner
Version:    0.4.0
Release:    1
Group:      Qt/Qt
License:    BSD-3
URL:        https://github.com/rinigus/qt-runner
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9

Requires:   opt-qt5-sfos-maliit-platforminputcontext
Requires:   opt-qt5-qtwayland
Requires:   qqc2-breeze-style
Requires:   opt-plasma-integration

BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Compositor) >= 5.6.0
BuildRequires:  pkgconfig(Qt5Svg)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  qt5-qttools-linguist

%description
Wrapper for running Qt 5.15 apps on Sailfish.

PackageName: Qt Runner
Type: desktop-application
Categories:
  - Utility
Custom:
  Repo: https://github.com/rinigus/qt-runner
Icon: https://raw.githubusercontent.com/rinigus/qt-runner/main/icons/qt-runner.svg


%prep
%setup -q -n %{name}-%{version}

%build
%qtc_qmake5 ARCH=%{_arch} VERSION='%{version}-%{release}'
%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
%{_datadir}/%{name}/translations
