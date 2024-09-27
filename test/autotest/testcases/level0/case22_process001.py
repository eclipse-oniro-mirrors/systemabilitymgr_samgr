#!/usr/bin/env python3
#-*- coding: utf-8 -*-

# Copyright (c) 2024 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from devicetest.core.test_case import TestCase, CheckPoint
from hypium import UiDriver
from tools.get_source_path import get_source_path
from tools.push_remove_source import push_source


class case22_process001(TestCase):

    def __init__(self, configs):
        self.TAG = self.__class__.__name__
        TestCase.__init__(self, self.TAG, configs)
        self.tests = [
            "test_step"
        ]
        self.driver = UiDriver(self.device1)
        self.sn = self.device1.device_sn

    def setup(self):
        self.log.info("case22_process001 start")
        need_source = {"cfg": False, "fwk": False, "listen_test": False, "audio_ability": False, "ondemand": True,
                       "proxy": False, "para": False}
        source_path = get_source_path(need_source=need_source, casename="level0/case22_process001")
        push_source(source_path=source_path, driver=self.driver, sn=self.sn)

    def test_step(self):
        driver = self.driver
        CheckPoint("Print process information")
        result = driver.System.execute_command("ondemand proc getp")
        assert "GetRunningSystemProcess size" in result

    def teardown(self):
        self.log.info("case22_process001 down")
