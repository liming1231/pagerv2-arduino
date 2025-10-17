#ifndef __AX_WEBSERVER_H__
#define __AX_WEBSERVER_H__

#include <WebServer.h>
#include <WiFi.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

typedef struct _web_infos {
  String sn;
  String version;
  String web_lang;
  String deploy_lang;
  String ssid;
  String password;
  String ip;
  String rssi;
  String deploy_env;
  String mqtt_server;
  String mqtt_online;
  String mqtt_server_ip;
  String dns;
  String ap_gw;
  String wpa2EnterpriseAuthMethod;
  String wpa2EnterpriseAuthIdentity;
  String wpa2EnterpriseAuthUsername;
  String priv_proto;
  String priv_addr;
  String priv_port;
  String priv_usr;
  String priv_pwd;
} web_infos;

extern web_infos webInfos;

void pthread_ap(void *);

const char router_index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="zh">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>PagerSetup</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: Roboto, "Helvetica Neue", Arial, sans-serif;
            background: #f5f5f5;
            color: #333;
            line-height: 1.5;
        }

        .c-box {
            max-width: 800px;
            margin: 20px auto;
            background: white;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
            padding: 20px;
        }

        .l-c {
            max-width: 400px;
            margin: 100px auto;
            background: white;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
            padding: 30px;
        }

        .l-t {
            text-align: center;
            margin-bottom: 20px;
            color: #2563eb;
            font-size: 24px;
        }

        .e-m {
            color: #dc2626;
            font-size: 14px;
            margin-top: 10px;
            text-align: center;
            display: none;
        }

        .tabs {
            display: flex;
            border-bottom: 1px solid #ddd;
            margin-bottom: 20px;
        }

        .tab {
            padding: 10px 0;
            margin-right: 20px;
            cursor: pointer;
            border: none;
            background: none;
            color: #666;
            font-size: 14px;
            position: relative;
        }

        .tab.active {
            color: #2563eb;
        }

        .tab.active::after {
            content: '';
            position: absolute;
            bottom: -1px;
            left: 0;
            width: 100%;
            height: 2px;
            background: #2563eb;
        }

        .t-c {
            display: none;
            padding: 20px 0;
        }

        .t-c.active {
            display: block;
            max-height: 500px;
            overflow-y: auto;
        }

        .f-g {
            margin-bottom: 15px;
        }

        label {
            display: block;
            margin-bottom: 5px;
            font-size: 14px;
            font-weight: 500;
        }

        input[type="text"],
        input[type="number"],
        input[type="password"],
        select {
            width: 100%;
            padding: 8px 12px;
            border: 1px solid #ddd;
            border-radius: 4px;
            font-size: 14px;
        }

        input[type="text"]:focus,
        input[type="number"],
        input[type="password"]:focus,
        select:focus {
            outline: none;
            border-color: #2563eb;
        }

        .checkbox-group {
            display: flex;
            align-items: center;
            gap: 8px;
        }

        .btn {
            background: #2563eb;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            cursor: pointer;
            font-size: 14px;
            width: 100%;
        }

        .btn:hover {
            background: #1d4ed8;
        }

        .footer {
            margin-top: 20px;
            padding-top: 20px;
            border-top: 1px solid #ddd;
            text-align: right;
        }

        .p-s-c {
            display: none;
            background: #f8f9fa;
            padding: 15px;
            border-radius: 4px;
            margin-top: 15px;
            border: 1px solid #ddd;
            margin-bottom: 10px;
        }

        .p-s-c.active {
            display: block;
        }

        #m-c {
            display: none;
        }

        .list-box {
            display: flex;
        }

        .list-box .btn {
            margin-left: 20px;
            width: 200px;
        }

        .update-box {
            width: 60%;
            position: relative;
        }

        #fileInput {
            opacity: 0;
            width: 60%;
            position: absolute;
            top: 0;
            height: 100%;
            z-index: 10;
            cursor: pointer;
        }

        #file-box {
            position: absolute;
            width: 100px;
            height: 30px;
            border-radius: 5px;
            top: 0;
            border: 1px solid #666666;
        }

        #file-info {
            position: absolute;
            top: 0px;
            left: 120px;
        }

        .progress-box {
            margin-top: 10px;
            display: flex;
            align-items: center;
            justify-content: flex-start;
        }

        #progress {
            height: 10px;
            background: #1d4ed8;
            border-radius: 5px;
            margin-right: 10px;
        }

        .t-b {
            display: flex;
            justify-content: center;
            align-items: center;
        }

        .t-min {
            font-size: 14px;
            line-height: 30px;
            margin-top: 10px;
            margin-left: 10px;
            color: #000000;
        }
        .f-b-i {
            display: flex;
            justify-content: flex-start;
            align-items: center;
        }
        .f-b-i input {
            margin:0 0 5px 10px;

        }
    </style>
</head>

<body>
    <!-- Login Form -->
    <div id="l-c" class="l-c">
        <h2 class="l-t" id="l-1"></h2>
        <div class="f-g">
            <label id="l-2"></label>
            <input type="text" id="username">
        </div>
        <div class="f-g">
            <label id="l-3"></label>
            <input type="password" id="pwd">
        </div>
        <button class="btn" id="l-4" onclick="handleLogin()"></button>
        <div id="e-m" id="l-5" class="e-m"></div>
    </div>

    <!-- Main Content (Initially Hidden) -->
    <div id="m-c" class="c-box">
        <div class="t-b">
            <h2 class="l-t" id="l-20"></h2> <span class="l-t t-min" id="version"></span>
            <!-- <h2 class="l-t" id="l-20"></h2>
            <h5 class="t-min"> version: <span class="t-min" id="version"></span></h5> -->
        </div>
        <div class="c-box">
            <div class="tabs">
                <button class="tab active" data-tab="info" id="l-6"></button>
                <button class="tab" data-tab="gateway" id="l-8"></button>
                <button class="tab" data-tab="wifi" id="l-10"></button>
                <button class="tab" data-tab="deploy" id="l-27"></button>
                <button class="tab" data-tab="system" id="l-14"></button>
                <button class="tab" data-tab="update" id="l-36"></button>
            </div>

            <div id="info" class="t-c active">
                <div class="system-info"></div>
            </div>

            <div id="gateway" class="t-c">
                <div class="f-g">
                    <label id="l-24"></label>
                    <select id="ap_gw"></select>
                </div>
            </div>

            <div id="wifi" class="t-c">
                <div class="f-g">
                    <label>SSID</label>
                    <input type="text" id="SSID">
                </div>
                <div class="f-g">
                    <label id="l-30"></label>
                    <input type="password" id="Password">
                </div>
                <div class="f-g">
                    <label id="l-11"></label>
                    <select id="encryption" onchange="EncryptionChange()"></select>
                </div>
                <div id="WPA2_ENTERPRISE" class="p-s-c">
                    <div class="f-g">
                        <label id="l-12"></label>
                        <select id="protocol"></select>
                    </div>
                    <div class="f-g">
                        <label>wpa2_identity</label>
                        <input type="text" id="wpa2_identity">
                    </div>
                    <div class="f-g">
                        <label>wpa2_username</label>
                        <input type="text" id="wpa2_username">
                    </div>
                </div>
                <div class="f-g f-b-i">
                    <label>Static_ip:</label>
                    <input type="checkbox" onchange="staticIpChange()" id="Static_ip">
                </div>
                <div id="Static_ip-Box" class="p-s-c">
                    <div class="f-g">
                        <label>ip</label>
                        <input type="text" id="Static_ip_addr">
                    </div>
                    <div class="f-g">
                        <label id="l-39"></label>
                        <input type="text" id="Static_gateway">
                    </div>
                    <div class="f-g">
                        <label id="l-40"></label>
                        <input type="text" id="Static_subnet">
                    </div>
                </div>
                <div class="f-g">
                    <label id="l-13"></label>
                    <input type="text" id="dns_server">
                </div>
            </div>

            <div id="deploy" class="t-c">
                <div class="f-g">
                    <label id="l-15"></label>
                    <select id="deploy_lang"></select>
                </div>

                <div class="f-g">
                    <label id="l-16"></label>
                    <select id="deploy_env" onchange="AccessPointChange()"></select>
                </div>

                <div id="p-s-c" class="p-s-c">
                    <div class="f-g">
                        <label id="l-21"></label>
                        <select id="dagreement_type"></select>
                    </div>
                    <div class="f-g">
                        <label id="l-17"></label>
                        <input type="text" id="priv_addr">
                    </div>
                    <div class="f-g">
                        <label id="l-18"></label>
                        <input type="number" id="priv_port">
                    </div>
                    <div class="f-g">
                        <label id="l-22"></label>
                        <input type="text" id="priv_usr">
                    </div>
                    <div class="f-g">
                        <label id="l-23"></label>
                        <input type="text" id="priv_pwd">
                    </div>

                </div>

                <!-- <div class="f-g">
                    <div class="checkbox-group">
                        <input type="checkbox" id="mac_filter">
                        <label for="mac_filter" id="l-19"></label>
                    </div>
                </div> -->
            </div>

            <div id="system" class="t-c">
                <div class="f-g">
                    <label id="l-31"></label>
                    <div class="list-box">
                        <select id="web_lang"></select>
                        <button class="btn" id="l-35" onclick="handleSave()"></button>
                    </div>
                </div>
                <div class="footer">
                    <button class="btn" id="l-7" onclick="handleRestart()"></button>
                </div>
                <div class="footer">
                    <button class="btn" id="l-28" onclick="initSetting()"></button>
                </div>
            </div>

            <div id="update" class="t-c">
                <div class="f-g">
                    <label id="l-37"></label>
                    <div class="list-box">
                        <div class="update-box">
                            <input type='file' name='update' id='fileInput' onchange="changeFile(this)">
                            <label id="file-box"></label>
                            <span id="file-info"></span>
                        </div>
                        <button class="btn" id="l-38" onclick="updateFile()"></button>
                    </div>
                    <div class="progress-box">
                        <div id="progress"></div>
                        <span id="status"></span>
                    </div>
                </div>

            </div>

            <div class="footer" id="save-btn">
                <button class="btn" id="l-9" onclick="handleSave()"></button>
            </div>

        </div>
    </div>

    <script>
        var LangMap = {
            '中文': { 1: '呼叫器登录', 2: '用户名', 3: '密码', 4: '登录', 5: '用户名或密码错误', 6: '系统信息', 7: '重启设备', 8: '网关设置', 9: '保存设置', 10: '网络设置', 11: '加密方式', 12: '加密协议', 13: 'dns服务器', 14: '系统设置', 15: '语言', 16: '接入点', 17: '服务器地址', 18: '端口', 19: '启用MAC过滤', 20: '呼叫器设置', 21: '协议类型', 25: '保存配置吗?', 26: '重启路由器吗?', 27: '部署设置', 28: '初始化设置', 29: '私有化信息不能为空!', 31: '系统语言', 33: '操作失败，请重试！', 34: '操作成功！', 35: '设置', 36: '升级', 37: '选择文件', 38: '版本号：', 39: '网关', 40: '子网掩码' },
            ENGLISH: { 1: 'Pager Login', 2: 'username', 3: 'password', 4: 'Login', 5: 'Username or password incorrect', 6: 'System Information', 7: 'Reboot', 8: 'Gateway Settings', 9: 'Save settings', 10: 'Network Settings', 11: 'Encryption', 12: 'Encryption Protocol', 13: 'DNS Server', 14: 'System settings', 15: 'Language Selection', 16: 'Access Point', 17: 'Server address', 18: 'Port', 19: 'Enable MAC filtering', 20: 'Pager Settings', 21: 'agreement type', 25: 'Are you sure to save the configuration?', 26: 'Are you sure to restart the pager?', 27: 'Deploy setting', 28: 'Factory settings', 29: 'Privatization information cannot be empty!', 31: 'System language', 33: 'Operation failed, please try again!', 34: 'Operation successful!', 35: 'Set', 36: 'update', 37: 'Select file', 38: 'version:', 39: 'Gateway', 40: 'Subnet Mask' }
        }
        // 统一配置对象
        const config = {
            // 系统信息
            curTab: 'info',
            // url: 'http://10.10.40.190:8088',
            url: 'http://' + document.location.host,
            times: null,
            pagesInfo: {
                'dns_server': 'wifi.dns_server',
                'deploy_lang': 'deploy.deploy_lang',
                'web_lang': 'system.web_lang',
                'deploy_env': 'deploy.deploy_env',
                'dagreement_type': 'deploy.dagreement_type',
                'priv_addr': 'deploy.priv_addr',
                'priv_port': 'deploy.priv_port',
                'priv_pwd': 'deploy.priv_pwd',
                'priv_usr': 'deploy.priv_usr',
                'mac_filter': 'deploy.mac_filter',
                'ap_gw': 'gateway.ap_gw',
                'SSID': 'wifi.SSID',
                'Password': 'wifi.Password',
                'encryption': 'wifi.encryption',
                'protocol': 'wifi.protocol',
                'wpa2_identity': 'wifi.wpa2_identity',
                'wpa2_username': 'wifi.wpa2_username',
                'version': 'version',
                'Static_ip': 'wifi.Static_ip',
                'Static_ip_addr': 'wifi.Static_ip_addr',
                'Static_gateway': 'wifi.Static_gateway',
                'Static_subnet': 'wifi.Static_subnet',
            },
            systemInfo: {},
            // 热点设置
            ap_gwOptions: [{ value: '10.25.25.1', label: '10.25.25.1' }, { value: '172.25.25.1', label: '172.25.25.1' }],
            encryptionOptions: ['WPA2_WPA3_PSK', 'WPA2_ENTERPRISE'],
            protocolOptions: ['WPA2_AUTH_TLS', 'WPA2_AUTH_PEAP', 'WPA2_AUTH_TTLS'],
            deploy_langOptions: [{ value: '中文', label: '中文' }, { value: 'ENGLISH', label: 'ENGLISH' }, { value: 'JAPANESE', label: 'JAPANESE' }],
            web_langOptions: [{ value: '中文', label: '中文' }, { value: 'ENGLISH', label: 'ENGLISH' }],
            deploy_envOptions: [{ value: 'DEVELOPMENT_ALI', label: 'DEVELOPMENT-ALI' }, { value: 'DEVELOPMENT_AX', label: 'DEVELOPMENT-AX' }, { value: 'PRODUCT_ALI', label: 'PRODUCT-ALI' }, { value: 'PRODUCT_AX', label: 'PRODUCT-AX' }, { value: 'FOREIGN_ALI', label: 'FOREIGN-ALI' }, { value: 'FOREIGN_AX', label: 'FOREIGN-AX' }, { value: 'EU_AX', label: 'EU-AX' }, { value: 'PRIVATE', label: 'PRIVATE' }, { value: 'JAPANESE', label: 'JAPANESE' }],
            dagreement_typeOptions: ['tcp', 'tls'],
            // 网关
            gateway: {
                ap_gw: '10.25.25.1'
            },
            // WiFi设置
            wifi: {
                SSID: '',
                Password: '',
                encryption: 'WPA2_WPA3_PSK',
                protocol: 'WPA2_AUTH_TLS',
                wpa2_identity: '',
                wpa2_username: '',
                dns_server: '',
                Static_ip: false,
                Static_ip_addr: '',
                Static_gateway: '',
                Static_subnet: '',
            },
            // 系统设置
            deploy: {
                deploy_lang: '中文',
                deploy_env: 'PRODUCT_ALI',
                dagreement_type: 'tcp',
                priv_addr: '',
                priv_port: '',
                priv_usr: '',
                priv_pwd: '',
                mac_filter: false
            },
            system: {
                web_lang: '中文',
            }
        };

        // ajax 请求
        function ajaxInstance() {
            let instance;
            return {
                getInstance: function () {
                    if (!instance) {
                        instance = new XMLHttpRequest();
                    }
                    return instance;
                },
                sendRequest: function (method, url, data, reWriteUrl) {
                    return new Promise((resolve, reject) => {
                        const xhr = this.getInstance();
                        url = (reWriteUrl || config.url) + url
                        if (method === 'GET' && data) {
                            url = url + `?` + Object.entries(data).map(([key, value]) => `${encodeURIComponent(key)}=${encodeURIComponent(value)}`).join("&")
                        }
                        xhr.open(method, url, true);
                        // 上传进度监听
                        xhr.upload.addEventListener('progress', function (event) {
                            if (event.lengthComputable) {
                                const percentComplete = (event.loaded / event.total) * 100;
                                const progressBar = document.getElementById('progress');
                                progressBar.style.width = percentComplete + '%';
                                // console.log(percentComplete, '===========')
                                document.getElementById('status').textContent = `${Math.round(percentComplete)}%`;
                            }
                        });
                        xhr.onreadystatechange = function () {
                            if (xhr.readyState === 4) {
                                if (xhr.status >= 200 && xhr.status < 300) {
                                    let response = null
                                    // console.log(xhr.responseText);
                                    if (xhr.responseText) {
                                        response = JSON.parse(xhr.responseText)
                                        catchErrorFunc(response)
                                    }
                                    resolve(response);
                                } else {
                                    reject(new Error(`Request failed with status: ${xhr.status}`), null);
                                }
                            }
                        };

                        xhr.send(method === 'POST' ? data : null);
                    })
                }
            }
        }
        initLang()
        const pwd = document.getElementById('pwd')

        // Login handling
        function handleLogin() {
            const username = document.getElementById('username').value;
            const errorMessage = document.getElementById('e-m');
            if (username === 'admin' && pwd.value === 'admin') {
                document.getElementById('l-c').style.display = 'none';
                document.getElementById('m-c').style.display = 'block';
                document.getElementById('save-btn').style.display = 'none'
                getNetInfo(true)
            } else {
                errorMessage.style.display = 'block';
                initLang(errorMessage, 5)
                pwd.value = '';
            }
        }

        // Add enter key support for login
        pwd.addEventListener('keypress', function (e) {
            if (e.key === 'Enter') {
                handleLogin();
            }
        });

        getNetInfo()

        function EncryptionChange() {
            const encryption = document.getElementById("encryption").value;
            document.getElementById('WPA2_ENTERPRISE').style.display = encryption === 'WPA2_ENTERPRISE' ? 'block' : 'none'
        }

        function staticIpChange() {
            const Static_ip = document.getElementById("Static_ip").checked;
            document.getElementById('Static_ip-Box').style.display = Static_ip ? 'block' : 'none'
        }

        function AccessPointChange() {
            const deploy_env = document.getElementById('deploy_env').value;
            const privateServerConfig = document.getElementById('p-s-c');
            if (deploy_env === 'PRIVATE') {
                privateServerConfig.classList.add('active');
            } else {
                privateServerConfig.classList.remove('active');
            }
        }

        function initLang(el, num) {
            const web_lang = localStorage.getItem('web_lang')
            const lang = web_lang || (['中文', 'ENGLISH'].includes(config.system.web_lang) ? config.system.web_lang : '中文')

            if (el) { el.innerHTML = LangMap[lang][num] }
            const repeat = { 22: 2, 23: 3, 30: 3, 24: 8, 32: 9, 38: 36 }
            for (let i = 1; i < 41; i++) {
                const dom = document.getElementById('l-' + i);
                if (dom) dom.innerHTML = LangMap[lang][repeat[i] || i]
            }
        }

        function getLang(num) {
            const web_lang = localStorage.getItem('web_lang')
            const lang = web_lang || (['中文', 'ENGLISH'].includes(config.system.web_lang) ? config.system.web_lang : '中文')
            if (num) { return LangMap[lang][num] } else return ''
        }

        // 初始化页面
        function initializePage(isUpdateInfo) {
            // 初始化系统信息
            const systemInfoHtml = Object.entries(config.systemInfo)
                .map(([key, value]) => `<p>${key}: ${value}</p>`)
                .join('');
            document.querySelector('.system-info').innerHTML = systemInfoHtml;
            if (isUpdateInfo) {
                for (let [k, v] of Object.entries(config.pagesInfo)) {
                    console.log(`${JSON.stringify(k)}: ${JSON.stringify(v)}`);
                    
                    const el = document.getElementById(k);
                    if (el) {
                        const keyArr = v.split('.');
                        const Options = config[k + 'Options']
                        if (Options && Options.length && el.options.length === 0) {
                            Options.forEach(config => {
                                el.add(new Option(config.label || config, config.value || config));
                            });
                        }
                        if (k === 'version') {
                            el.innerHTML = getLang(38) + config.systemInfo[k]
                            continue
                        }
                        console.log(config.pagesInfo[k]);
                        if (k === 'Static_ip') {
                            config[keyArr[0]][keyArr[1]] = el.checked = config.systemInfo[k]
                            continue
                        }
                        if (keyArr.length) {
                            config[keyArr[0]][keyArr[1]] = el.value = config.systemInfo[k] || config[keyArr[0]][keyArr[1]]
                        }
                    }
                }
                bindEvents();
            }
        }

        // 绑定事件
        function bindEvents() {
            // 标签切换
            const tabs = document.querySelectorAll('.tab');
            const contents = document.querySelectorAll('.t-c');
            if (['info', 'system', 'update'].includes(config.curTab)) {
                document.getElementById('save-btn').style.display = 'none'
            }
            tabs.forEach(tab => {
                tab.addEventListener('click', () => {
                    // console.log(tab.dataset.tab);
                    config.curTab = tab.dataset.tab
                    tabs.forEach(t => t.classList.remove('active'));
                    contents.forEach(c => c.classList.remove('active'));
                    tab.classList.add('active');
                    const content = document.getElementById(tab.dataset.tab);
                    // initializePage(true)
                    content.classList.add('active');
                    if (['info', 'system', 'update'].includes(tab.dataset.tab)) {
                        document.getElementById('save-btn').style.display = 'none'
                    } else {
                        document.getElementById('save-btn').style.display = 'block'
                    }
                });
            });

            // 监听所有输入变化
            document.querySelectorAll('input, select').forEach(element => {
                element.addEventListener('change', () => {
                    updateConfig();
                });
            });

            // 判断是否为 企业级wifi
            EncryptionChange()
            AccessPointChange()
            staticIpChange()

        }

        // 更新配置对象
        function updateConfig() {
            for (let [k, v] of Object.entries(config.pagesInfo)) {
                // console.log(`${JSON.stringify(k)}: ${JSON.stringify(v)}`);
                const el = document.getElementById(k);
                if (el) {
                    const keyArr = v.split('.');
                    if (keyArr.length === 2) {
                        if (k === 'Static_ip') {
                            config[keyArr[0]][keyArr[1]] = el.checked
                        }
                        else
                        {
                            config[keyArr[0]][keyArr[1]] = el.value;
                        }
                        
                    } else {
                        config[keyArr[0]] = el.value;
                    }
                }
            }
        }

        function getNetInfo(isUpdateInfo) {
            clearTimeout(config.times)
            ajaxInstance().sendRequest('GET', '/get_infos', {}).then((res) => {
                console.log(res)
                config.systemInfo = res
                initializePage(isUpdateInfo)
            })
            config.times = setTimeout(() => {
                getNetInfo()
            }, 5000)
        }

        // 处理保存
        function handleSave() {
            console.log(config);
            if (config.curTab === 'deploy' && config.deploy.deploy_env === 'PRIVATE' && (!config.deploy.priv_addr || !config.deploy.priv_port || !config.deploy.priv_usr || !config.deploy.priv_pwd)) {
                alert(getLang(29))
                return
            }
            if (confirm(getLang(25))) {
                if (config.curTab === 'deploy') {
                    ajaxInstance().sendRequest('GET', '/private_deploy', config[config.curTab]).then((res) => {
                    })
                } else if (config.curTab === 'wifi') {
                    ajaxInstance().sendRequest('GET', '/wificonfad', config[config.curTab]).then((res) => {
                    })
                } else if (config.curTab === 'gateway') {
                    ajaxInstance().sendRequest('GET', '/apgw_select', config[config.curTab]).then((res) => {
                    })
                } else if (config.curTab === 'system') {
                    localStorage.setItem('web_lang', config.system.web_lang)
                    ajaxInstance().sendRequest('GET', '/web_lang_select', config[config.curTab]).then((res) => {
                        initLang()
                    })
                }
                updateConfig();
                getNetInfo(true);
            }
        }

        // 处理重启
        function handleRestart() {
            // console.log(config)
            if (confirm(getLang(26))) {
                ajaxInstance().sendRequest('GET', '/reboot', { reboot: 'YES' }).then((res) => {
                    // console.log(res)
                })
            }
        }

        // 处理初始化
        function initSetting() {
            if (confirm(getLang(25))) {
                ajaxInstance().sendRequest('GET', '/factory_settings', { factory_settings: 'YES' }).then((res) => {
                })
            }
        }

        function catchErrorFunc(data) {
            // console.log(data);
            const response = Object.prototype.toString.call(data) === '[object String]' ? JSON.parse(data) : data
            if (response && response.res && response.res != 200) {
                alert(getLang(33))
            } else if (response && response.res && response.res == 200) {
                alert(getLang(34))
            }
        }

        function changeFile() {
            const fileInput = document.getElementById('fileInput');
            const file = fileInput.files[0];
            document.getElementById('file-info').innerHTML = '   ' + file.name;
        }

        function updateFile() {
            const fileInput = document.getElementById('fileInput');
            const file = fileInput.files[0];
            if (!file) {
                alert('请选择文件！');
                return;
            }
            const formData = new FormData();
            formData.append('file', file);
            // console.log(formData, file);
            //
            try {
                // 使用 Fetch API 上传文件
                ajaxInstance().sendRequest('POST', '/update', formData).then((res) => {
                    // console.log(res);
                })
            } catch (error) {
                console.error('上传失败', error);
                // document.getElementById('result').textContent = '上传失败，请重试。';
            }
        }

    </script>
</body>

</html>
)rawliteral";

#endif
