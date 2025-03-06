#!/usr/bin/env python3
"""
Skript pro automaticke testovani prvni casti projektu.

Spousteni:
   pytest
nebo
   python3 -m pytest
"""
import part01
import numpy as np
import os
import pytest


def test_integrate():
    """Test vypoctu integralu """

    r = part01.distance(
        np.array([[0, 0], [0, 0], [2, 2]]),
        np.array([[1, 1], [2, 2], [5, 6]])
    )

    np.testing.assert_allclose(r, [1.41421356, 2.82842712, 5.])


def test_generate_fn():
    """Test generovani grafu s vice funkcemi"""
    part01.generate_graph([7, 4, 3], show_figure=False,
                          save_path="tmp_fn.png")
    assert os.path.exists("tmp_fn.png")


def test_generate_sin():
    """Test generovani grafu se sinusovkami"""
    part01.generate_sinus(show_figure=False, save_path="tmp_sin.png")
    assert os.path.exists("tmp_sin.png")


def test_download():
    """Test stazeni dat"""
    data = part01.download_data()

    assert len(data["positions"]) == 40
    assert len(data["lats"]) == 40
    assert len(data["longs"]) == 40
    assert len(data["heights"]) == 40

    assert data["positions"][0] == "Cheb"
    assert data["lats"][0] == pytest.approx(50.0683)
    assert data["longs"][0] == pytest.approx(12.3913)
    assert data["heights"][0] == pytest.approx(483.0)
