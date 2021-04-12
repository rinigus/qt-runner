Name:       flatpak-runner

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    Flatpak runner
Version:    0.8.6
Release:    1
Group:      Qt/Qt
License:    BSD-3
URL:        https://github.com/rinigus/flatpak-runner
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   rsync
Requires:   flatpak
Requires:   xdg-desktop-portal > 0.10
Requires:   pyotherside-qml-plugin-python3-qt5 >= 1.5.1
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Compositor)
BuildRequires:  pkgconfig(Qt5Svg)
BuildRequires:  qt5-qttools-linguist

%description
Wrapper for running Flatpak apps on Sailfish.


%prep
%setup -q -n %{name}-%{version}

%build
%qtc_qmake5 VERSION='%{version}-%{release}'
%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

chmod 755 %{buildroot}/%{_datadir}/%{name}/scripts/flatpak-extension-hybris

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
