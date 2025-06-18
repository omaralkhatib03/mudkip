import os
import requests
from bs4 import BeautifulSoup
import time

# Constants
BASE_URL = "https://sparse.tamu.edu"
BASE_DOWNLOAD_URL = "https://suitesparse-collection-website.herokuapp.com/MM/"
START_PAGE = 59
END_PAGE = 75
MAX_SIZE =   98304 # Adjust this value as needed
OUTPUT_DIR = "downloaded_matrices"
HEADERS = {'User-Agent': 'Mozilla/5.0'}

# Create output directory if it doesn't exist
os.makedirs(OUTPUT_DIR, exist_ok=True)

def fetch_page(url):
    try:
        response = requests.get(url, headers=HEADERS)
        response.raise_for_status()
        return response.text
    except requests.RequestException as e:
        print(f"Error fetching {url}: {e}")
        return None

def parse_main_page(html):
    soup = BeautifulSoup(html, 'html.parser')
    table = soup.find('table')
    if not table:
        print("No table found on the page.")
        return []

    rows = table.find_all('tr')[1:]  # Skip header row
    entries = []
    for row in rows:
        cols = row.find_all('td')
        if len(cols) < 6:
            continue
        try:
            name = cols[1].get_text(strip=True)
            group = cols[2].get_text(strip=True)
            rows_count = int(cols[3].get_text(strip=True).replace(',', ''))
            cols_count = int(cols[4].get_text(strip=True).replace(',', ''))
            kind = cols[6].get_text(strip=True)
            link_tag = cols[1].find('a')
            if not link_tag:
                continue
            detail_url = BASE_URL + link_tag['href']
            entries.append({
                'name': name,
                'kind': kind,
                'rows': rows_count,
                'cols': cols_count,
                'url': detail_url,
                'group': group
            })
        except Exception as e:
            print(f"Error parsing row: {e}")
            continue
    return entries

def download_mat_file(entry, name):
    # print(entry)
    # html = fetch_page(entry['url'])
    url =  BASE_DOWNLOAD_URL + f"{entry['group']}/{entry['name']}.tar.gz"
    # if not html:
        # return
    # soup = BeautifulSoup(html, 'html.parser')
    # download_links = soup.find_all('a', href=True)
    # for link in download_links:
    #     href = link['href']
    #     if href.endswith('.mat'):
    #         file_url = BASE_URL + href
    file_path = os.path.join(OUTPUT_DIR, f"{name}.tar.gz")
            # if os.path.exists(file_path):
                # print(f"{file_path} already exists. Skipping download.")
                # return
    try:
        print(f"Downloading {url} to {file_path}")
        with requests.get(url, headers=HEADERS, stream=True) as r:
            r.raise_for_status()
            with open(file_path, 'wb') as f:
                for chunk in r.iter_content(chunk_size=8192):
                    f.write(chunk)
        print(f"Downloaded {file_path}")
    except requests.RequestException as e:
        print(f"Error downloading {url}: {e}")
        return

def main():
    page = START_PAGE
    while page != True:
        print(f"Processing page {page}")
        page_url = f"{BASE_URL}/?filterrific%5Bsorted_by%5D=kind_asc&page={page}"
        html = fetch_page(page_url)
        if not html:
            break
        entries = parse_main_page(html)
        if not entries:
            print("No more entries found. Exiting.")
            break
        for entry in entries: 
            if (entry['kind'] == 'Linear Programming Problem' and
                entry['rows'] < MAX_SIZE and
                entry['cols'] < MAX_SIZE):
                print(f"Processing {entry['name']}: Rows={entry['rows']}, Cols={entry['cols']}")
                download_mat_file(entry, entry['name'])
                # time.sleep(1)
        page += 1
        # time.sleep(2)

if __name__ == "__main__":
    main()
